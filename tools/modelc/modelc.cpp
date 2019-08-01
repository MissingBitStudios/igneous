#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <bgfx/bgfx.h>
#include <bx/commandline.h>

typedef std::vector<uint8_t> attribute_list;

void logMsg(std::string msg)
{
	std::cout << msg << "\n";
}

int ctoi(char c)
{
	return c - '0';
}

int main(int argc, char** argv)
{
	bx::CommandLine cmd(argc, argv);

	const char* vertexFilePath = cmd.findOption('v', "vertex");
	if (vertexFilePath == NULL)
	{
		logMsg("-v Vertex layout must be provided.");
		return 1;
	}

	const char* modelFilePath = cmd.findOption('m', "Model");
	if (modelFilePath == NULL)
	{
		logMsg("-m Model file must be provided.");
		return 1;
	}

	const char* outputFilePath = cmd.findOption('o', "output");
	if (outputFilePath == NULL)
	{
		logMsg("-o Output file must be provided.");
		return 1;
	}

	attribute_list attributeList;
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
				logMsg("Value after TEXCOORD must be in the range 0-7");
				return 1;
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
				logMsg("Value after COLOR must be in the range 0-3");
				return 1;
			}
		}
		else
		{
			logMsg("Unrecognized vertex attribute.");
			return 1;
		}
	}
	vertexFile.close();

	Assimp::Importer importer;
	importer.SetPropertyString(AI_CONFIG_PP_OG_EXCLUDE_LIST, "lod0 lod1 lod2 lod3 collision");
	const aiScene* scene = importer.ReadFile(modelFilePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices | aiProcess_OptimizeGraph | aiProcess_OptimizeMeshes);
	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		logMsg("Could not open model file");
		return 1;
	}

	std::ofstream outputFile(outputFilePath, std::ios::out | std::ios::binary);

	uint64_t collisionOffset = 0;
	outputFile.write((char*)&collisionOffset, sizeof(uint64_t));

	uint8_t attributeListSize = (uint8_t)attributeList.size();
	outputFile.write((char*)&attributeListSize, sizeof(uint8_t));
	outputFile.write((char*)attributeList.data(), attributeListSize * sizeof(uint8_t));

	aiNode* lod0 = scene->mRootNode->FindNode("lod0");
	if (lod0 == NULL)
	{
		logMsg("Could not find lod0 mesh. It is required.");
		return 1;
	}

	logMsg("Writing lod0 model");
	unsigned int numMeshes = lod0->mNumMeshes;
	std::cout << "Mesh count: " << numMeshes << "\n";
	outputFile.write((char*)&numMeshes, sizeof(unsigned int));

	for (unsigned int meshIndex = 0; meshIndex < numMeshes; meshIndex++)
	{
		aiMesh* mesh = scene->mMeshes[lod0->mMeshes[meshIndex]];
		unsigned int numIndicies = mesh->mNumFaces * 3;
		std::cout << "Mesh [" << meshIndex << "] vertex count: " << mesh->mNumVertices << "\n";
		outputFile.write((char*)&mesh->mNumVertices, sizeof(unsigned int));
		std::cout << "Mesh [" << meshIndex << "] index count: " << numIndicies << "\n";
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
	if (collision == NULL)
	{
		logMsg("Could not find collision mesh. Skipping...");
	}
	else
	{
		collisionOffset = outputFile.tellp();
		outputFile.seekp(0, std::ios::beg);
		outputFile.write((char*)&collisionOffset, sizeof(uint64_t));
		outputFile.seekp(collisionOffset, std::ios::beg);

		logMsg("Writing collision mesh");

		aiMesh* mesh = scene->mMeshes[collision->mMeshes[0]];
		unsigned int numIndicies = mesh->mNumFaces * 3;
		std::cout << "Collision Mesh vertex count: " << mesh->mNumVertices << "\n";
		outputFile.write((char*)&mesh->mNumVertices, sizeof(unsigned int));

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			outputFile.write((char*)&mesh->mVertices[i].x, sizeof(float));
			outputFile.write((char*)&mesh->mVertices[i].y, sizeof(float));
			outputFile.write((char*)&mesh->mVertices[i].z, sizeof(float));
		}
	}
	
	outputFile.close();
	
	return 0;
}
