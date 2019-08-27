#include "ziggurat.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <bgfx/bgfx.h>

const std::unordered_map<std::string, bgfx::Attrib::Enum> attribLookupTable = {
	{"POSITION", bgfx::Attrib::Position},
	{"NORMAL", bgfx::Attrib::Normal},
	{"TANGENT", bgfx::Attrib::Tangent},
	{"BITANGENT", bgfx::Attrib::Bitangent},
	{"COLOR0", bgfx::Attrib::Color0},
	{"COLOR1", bgfx::Attrib::Color1},
	{"COLOR2", bgfx::Attrib::Color2},
	{"COLOR3", bgfx::Attrib::Color3},
	{"INDICES", bgfx::Attrib::Indices},
	{"WEIGHT", bgfx::Attrib::Weight},
	{"TEXCOORD0", bgfx::Attrib::TexCoord0},
	{"TEXCOORD1", bgfx::Attrib::TexCoord1},
	{"TEXCOORD2", bgfx::Attrib::TexCoord2},
	{"TEXCOORD3", bgfx::Attrib::TexCoord3},
	{"TEXCOORD4", bgfx::Attrib::TexCoord4},
	{"TEXCOORD5", bgfx::Attrib::TexCoord5},
	{"TEXCOORD6", bgfx::Attrib::TexCoord6},
	{"TEXCOORD7", bgfx::Attrib::TexCoord7}
};

bool Ziggurat::compileModel(const std::string& name)
{
	std::filesystem::path modelConfigPath = sourceResDir / "models" / (name + ".mdl");

	if (!stampList->isOutOfDate(modelConfigPath))
	{
		return true;
	}

	std::ifstream modelConfigFile(modelConfigPath);
	std::string modelPath, vertexName, lod0Name, collisionName;

	std::string param;
	while (modelConfigFile >> param)
	{
		if (param == "MODEL")
		{
			modelConfigFile >> modelPath;
		}
		else if (param == "VERTEX")
		{
			modelConfigFile >> vertexName;
		}
		else if (param == "LOD")
		{
			modelConfigFile >> lod0Name;
		}
		else if (param == "COLLISION")
		{
			modelConfigFile >> collisionName;
		}
	}

	std::vector<uint8_t> attributeList;
	std::ifstream vertexFile(sourceResDir / "vertex" / (vertexName + ".vtx"), std::ios::in);
	std::string token;
	while (vertexFile >> token)
	{
		attributeList.push_back(attribLookupTable.at(token));
	}
	vertexFile.close();

	Assimp::Importer importer;
	importer.SetPropertyString(AI_CONFIG_PP_OG_EXCLUDE_LIST, lod0Name + " " + collisionName);
	const aiScene* scene = importer.ReadFile((sourceResDir / "models" / modelPath).string(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices | aiProcess_OptimizeGraph | aiProcess_OptimizeMeshes);
	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cerr << "Could not open model file" << std::endl;
		return false;
	}

	std::ofstream outputFile(binaryResDir / "models" / (name + ".bin"), std::ios::out | std::ios::binary);

	uint64_t collisionOffset = 0;
	outputFile.write((char*)&collisionOffset, sizeof(uint64_t));

	uint8_t attributeListSize = (uint8_t)attributeList.size();
	outputFile.write((char*)&attributeListSize, sizeof(uint8_t));
	outputFile.write((char*)attributeList.data(), attributeListSize * sizeof(uint8_t));

	aiNode* lod0 = scene->mRootNode->FindNode(lod0Name.c_str());
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
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::string materialName(material->GetName().C_Str());
		size_t materialNameLength = materialName.length();
		outputFile.write((char*)&materialNameLength, sizeof(size_t));
		outputFile.write(materialName.c_str(), materialNameLength);
	}

	for (unsigned int meshIndex = 0; meshIndex < numMeshes; meshIndex++)
	{
		aiMesh* mesh = scene->mMeshes[lod0->mMeshes[meshIndex]];

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			for (uint8_t attribute : attributeList)
			{
				if (attribute == bgfx::Attrib::Position)
				{
					outputFile.write((char*)& mesh->mVertices[i].x, sizeof(float));
					outputFile.write((char*)& mesh->mVertices[i].y, sizeof(float));
					outputFile.write((char*)& mesh->mVertices[i].z, sizeof(float));
				}
				else if (attribute == bgfx::Attrib::Normal)
				{
					outputFile.write((char*)&mesh->mNormals[i].x, sizeof(float));
					outputFile.write((char*)&mesh->mNormals[i].y, sizeof(float));
					outputFile.write((char*)&mesh->mNormals[i].z, sizeof(float));
				}
				else if (attribute == bgfx::Attrib::Tangent)
				{
						outputFile.write((char*)&mesh->mTangents[i].x, sizeof(float));
					outputFile.write((char*)&mesh->mTangents[i].y, sizeof(float));
					outputFile.write((char*)&mesh->mTangents[i].z, sizeof(float));
				}
				else if (attribute == bgfx::Attrib::Bitangent)
				{
					outputFile.write((char*)&mesh->mBitangents[i].x, sizeof(float));
					outputFile.write((char*)&mesh->mBitangents[i].y, sizeof(float));
					outputFile.write((char*)&mesh->mBitangents[i].z, sizeof(float));
				}
				else if (attribute >= bgfx::Attrib::TexCoord0 && attribute <= bgfx::Attrib::TexCoord7)
				{
					outputFile.write((char*)&mesh->mTextureCoords[attribute - bgfx::Attrib::TexCoord0][i].x, sizeof(float));
					outputFile.write((char*)&mesh->mTextureCoords[attribute - bgfx::Attrib::TexCoord0][i].y, sizeof(float));
				}
				else if (attribute >= bgfx::Attrib::Color0 && attribute <= bgfx::Attrib::Color3)
				{
					uint8_t color[4];
					color[0] = mesh->mColors[attribute - bgfx::Attrib::Color0][i].r * 255;
					color[1] = mesh->mColors[attribute - bgfx::Attrib::Color0][i].g * 255;
					color[2] = mesh->mColors[attribute - bgfx::Attrib::Color0][i].b * 255;
					color[3] = mesh->mColors[attribute - bgfx::Attrib::Color0][i].a * 255;
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

	aiNode* collision = scene->mRootNode->FindNode(collisionName.c_str());
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
