#include "ziggurat.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <bgfx/bgfx.h>

int ctoi(char c)
{
	return c - '0';
}

bool compileModel(const std::filesystem::path& vertexFilePath, const std::filesystem::path& modelFilePath, const std::filesystem::path& outputFilePath)
{
	std::filesystem::create_directories(outputFilePath.parent_path());
	std::vector<uint8_t> attributeList;
	std::ifstream vertexFile(vertexFilePath, std::ios::in);
	std::string token;
	while (vertexFile >> token)
	{
		if (token == "POSITION")
		{
			attributeList.push_back(bgfx::Attrib::Position);
		}
		else if (token == "NORMAL")
		{
			attributeList.push_back(bgfx::Attrib::Normal);
		}
		else if (token == "TANGENT")
		{
			attributeList.push_back(bgfx::Attrib::Tangent);
		}
		else if (token == "BITANGENT")
		{
			attributeList.push_back(bgfx::Attrib::Bitangent);
		}
		else if (!token.rfind("TEXCOORD", 0) && token.length() == 9)
		{
			int index = ctoi(token.back());
			if (index >= 0 && index <= 7)
			{
				attributeList.push_back(bgfx::Attrib::TexCoord0 + index);
			}
			else
			{
				std::cerr << "Value after TEXCOORD must be in the range 0-7" << std::endl;
				return false;
			}
		}
		else if (!token.rfind("COLOR", 0) && token.length() == 6)
		{
			int index = ctoi(token.back());
			if (index >= 0 && index <= 3)
			{
				attributeList.push_back(bgfx::Attrib::Color0 + index);
			}
			else
			{
				std::cerr << "Value after COLOR must be in the range 0-3" << std::endl;
				return false;
			}
		}
		else
		{
			std::cerr << "Unrecognized vertex attribute." << std::endl;
			return false;
		}
	}
	vertexFile.close();

	Assimp::Importer importer;
	importer.SetPropertyString(AI_CONFIG_PP_OG_EXCLUDE_LIST, "lod0 lod1 lod2 lod3 collision");
	const aiScene* scene = importer.ReadFile(modelFilePath.string(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices | aiProcess_OptimizeGraph | aiProcess_OptimizeMeshes);
	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cerr << "Could not open model file" << std::endl;
		return false;
	}

	std::ofstream outputFile(outputFilePath, std::ios::out | std::ios::binary);

	uint64_t collisionOffset = 0;
	outputFile.write((char*)&collisionOffset, sizeof(uint64_t));

	uint8_t attributeListSize = (uint8_t)attributeList.size();
	outputFile.write((char*)&attributeListSize, sizeof(uint8_t));
	outputFile.write((char*)attributeList.data(), attributeListSize * sizeof(uint8_t));

	aiNode* lod0 = scene->mRootNode->FindNode("lod0");
	if (!lod0)
	{
		std::cerr << "Could not find lod0 mesh. It is required." << std::endl;
		return false;
	}

	unsigned int numMeshes = lod0->mNumMeshes;
	outputFile.write((char*)&numMeshes, sizeof(unsigned int));

	for (unsigned int meshIndex = 0; meshIndex < numMeshes; meshIndex++)
	{
		aiMesh* mesh = scene->mMeshes[lod0->mMeshes[meshIndex]];
		unsigned int numIndicies = mesh->mNumFaces * 3;
		outputFile.write((char*)&mesh->mNumVertices, sizeof(unsigned int));
		outputFile.write((char*)&numIndicies, sizeof(unsigned int));
	}

	for (unsigned int meshIndex = 0; meshIndex < numMeshes; meshIndex++)
	{
		aiMesh* mesh = scene->mMeshes[lod0->mMeshes[meshIndex]];

		const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		aiColor4D diffuse;
		bool hasMtlColor = AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse);

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			for (uint8_t attribute : attributeList)
			{
				if (attribute == bgfx::Attrib::Position)
				{
					if (mesh->mVertices)
					{
						outputFile.write((char*)& mesh->mVertices[i].x, sizeof(float));
						outputFile.write((char*)& mesh->mVertices[i].y, sizeof(float));
						outputFile.write((char*)& mesh->mVertices[i].z, sizeof(float));
					}
				}
				else if (attribute == bgfx::Attrib::Normal)
				{
					if (mesh->mNormals)
					{
						outputFile.write((char*)&mesh->mNormals[i].x, sizeof(float));
						outputFile.write((char*)&mesh->mNormals[i].y, sizeof(float));
						outputFile.write((char*)&mesh->mNormals[i].z, sizeof(float));
					}
				}
				else if (attribute == bgfx::Attrib::Tangent)
				{
					if (mesh->mTangents)
					{
						outputFile.write((char*)&mesh->mTangents[i].x, sizeof(float));
						outputFile.write((char*)&mesh->mTangents[i].y, sizeof(float));
						outputFile.write((char*)&mesh->mTangents[i].z, sizeof(float));
					}
				}
				else if (attribute == bgfx::Attrib::Bitangent)
				{
					if (mesh->mBitangents)
					{
						outputFile.write((char*)&mesh->mBitangents[i].x, sizeof(float));
						outputFile.write((char*)&mesh->mBitangents[i].y, sizeof(float));
						outputFile.write((char*)&mesh->mBitangents[i].z, sizeof(float));
					}
				}
				else if (attribute >= bgfx::Attrib::TexCoord0 && attribute <= bgfx::Attrib::TexCoord7)
				{
					if (mesh->mTextureCoords[attribute - bgfx::Attrib::TexCoord0])
					{
						outputFile.write((char*)&mesh->mTextureCoords[attribute - bgfx::Attrib::TexCoord0][i].x, sizeof(float));
						outputFile.write((char*)&mesh->mTextureCoords[attribute - bgfx::Attrib::TexCoord0][i].y, sizeof(float));
					}
				}
				else if (attribute >= bgfx::Attrib::Color0 && attribute <= bgfx::Attrib::Color3)
				{
					uint8_t color[4];

					if (hasMtlColor)
					{
						color[0] = diffuse.r * 255;
						color[1] = diffuse.g * 255;
						color[2] = diffuse.b * 255;
						color[3] = diffuse.a * 255;
					}
					else if (mesh->mColors[attribute - bgfx::Attrib::Color0])
					{
						color[0] = mesh->mColors[attribute - bgfx::Attrib::Color0][i].r * 255;
						color[1] = mesh->mColors[attribute - bgfx::Attrib::Color0][i].g * 255;
						color[2] = mesh->mColors[attribute - bgfx::Attrib::Color0][i].b * 255;
						color[3] = mesh->mColors[attribute - bgfx::Attrib::Color0][i].a * 255;
					}

					outputFile.write((char*)&color, sizeof(color));
				}
			}
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				uint16_t index = face.mIndices[j];
				outputFile.write((char*)&index, sizeof(uint16_t));
			}
		}
	}

	aiNode* collision = scene->mRootNode->FindNode("collision");
	if (collision)
	{
		collisionOffset = outputFile.tellp();
		outputFile.seekp(0, std::ios::beg);
		outputFile.write((char*)&collisionOffset, sizeof(uint64_t));
		outputFile.seekp(collisionOffset, std::ios::beg);

		aiMesh* mesh = scene->mMeshes[collision->mMeshes[0]];
		unsigned int numIndicies = mesh->mNumFaces * 3;
		outputFile.write((char*)&mesh->mNumVertices, sizeof(unsigned int));

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			outputFile.write((char*)&mesh->mVertices[i].x, sizeof(float));
			outputFile.write((char*)&mesh->mVertices[i].y, sizeof(float));
			outputFile.write((char*)&mesh->mVertices[i].z, sizeof(float));
		}
	}
	
	outputFile.close();
	
	return true;
}
