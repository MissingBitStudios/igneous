#pragma once

#include <string>
#include <type_traits>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "igneous/renderer/vertex.hpp"

namespace igneous {
namespace renderer
{
	template<typename VertexType>
	ModelHandle loadModel(std::string path, bgfx::ProgramHandle program)
	{
		static_assert(std::is_base_of<Vertex, VertexType>::value, "VertexType must derive from Vertex");

		if (models.count(path))
		{
			return models.at(path);
		}
		// read file via ASSIMP
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices | aiProcess_OptimizeGraph | aiProcess_OptimizeMeshes);
		// check for errors
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			return nullptr;
		}

		std::string directory = path.substr(0, path.find_last_of('/') + 1);

		ModelHandle model = new Model;
		model->program = program;

		const auto processMesh = [&](aiMesh* mesh)
		{
			Mesh buildMesh;

			std::vector<VertexType>* vertices = new std::vector<VertexType>;
			std::vector<uint16_t>* indices = new std::vector<uint16_t>;

			const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			aiColor4D diffuse;
			bool hasMtlColor = AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse);

			for (unsigned int i = 0; i < mesh->mNumVertices; i++)
			{
				VertexData vertexData;
				vertexData.pos.x = mesh->mVertices[i].x;
				vertexData.pos.y = mesh->mVertices[i].y;
				vertexData.pos.z = mesh->mVertices[i].z;
				// normals
				if (mesh->mNormals)
				{
					vertexData.norm.x = mesh->mNormals[i].x;
					vertexData.norm.y = mesh->mNormals[i].y;
					vertexData.norm.z = mesh->mNormals[i].z;
				}
				// texture coordinates
				if (mesh->mTextureCoords[0])
				{
					vertexData.tex.x = mesh->mTextureCoords[0][i].x;
					vertexData.tex.y = mesh->mTextureCoords[0][i].y;
				}
				// colors
				if (hasMtlColor)
				{
					vertexData.col.r = diffuse.r;
					vertexData.col.g = diffuse.g;
					vertexData.col.b = diffuse.b;
					vertexData.col.a = diffuse.a;
				}
				else if (mesh->mColors[0])
				{
					vertexData.col.r = mesh->mColors[0][i].r;
					vertexData.col.g = mesh->mColors[0][i].g;
					vertexData.col.b = mesh->mColors[0][i].b;
					vertexData.col.a = mesh->mColors[0][i].a;
				}
				// tangent
				if (mesh->mTangents)
				{
					vertexData.tan.x = mesh->mTangents[i].x;
					vertexData.tan.y = mesh->mTangents[i].y;
					vertexData.tan.z = mesh->mTangents[i].z;
				}
				// bitangent
				if (mesh->mBitangents)
				{
					vertexData.bi.x = mesh->mBitangents[i].x;
					vertexData.bi.y = mesh->mBitangents[i].y;
					vertexData.bi.z = mesh->mBitangents[i].z;
				}

				vertices->push_back(vertexData);
			}
			// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
			for (unsigned int i = 0; i < mesh->mNumFaces; i++)
			{
				aiFace face = mesh->mFaces[i];
				// retrieve all indices of the face and store them in the indices vector
				for (unsigned int j = 0; j < face.mNumIndices; j++)
				{
					indices->push_back(face.mIndices[j]);
				}
			}

			const int typesCount = 4;
			const aiTextureType types[typesCount] = { aiTextureType_DIFFUSE, aiTextureType_SPECULAR, aiTextureType_HEIGHT, aiTextureType_AMBIENT };
			for (int typeIndex = 0; typeIndex < typesCount; typeIndex++)
			{
				for (unsigned int i = 0; i < material->GetTextureCount(types[typeIndex]); i++)
				{
					aiString str;
					material->GetTexture(types[typeIndex], i, &str);
					buildMesh.textures.push_back(loadTexture(directory + str.C_Str()));
				}
			}

			buildMesh.vbh = bgfx::createVertexBuffer(bgfx::makeRef(vertices->data(), (uint32_t)vertices->size() * VertexType::ms_decl.getStride()), VertexType::ms_decl);
			buildMesh.ibh = bgfx::createIndexBuffer(bgfx::makeRef(indices->data(), (uint32_t)indices->size() * sizeof(uint16_t)));

			return buildMesh;
		};

		std::function<void(aiNode * node)> processNode;
		processNode = [&](aiNode* node)
		{
			for (unsigned int i = 0; i < node->mNumMeshes; i++)
			{
				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
				model->meshes.push_back(processMesh(mesh));
			}

			for (unsigned int i = 0; i < node->mNumChildren; i++)
			{
				processNode(node->mChildren[i]);
			}
		};

		// process ASSIMP's root node recursively
		processNode(scene->mRootNode);

		models[path] = model;

		return model;
	}
}
}
