#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <bgfx/bgfx.h>
#include <bx/commandline.h>

enum Attributes : uint8_t
{
	POSITION,
	NORMAL,
	TANGENT,
	BITANGENT,
	TEXTURE_COORDINATES,
	COLOR,
	NUM_ATTRIBUTES
};

void logMsg(std::string msg)
{
	std::cout << msg << "\n";
}

int ctoi(char c)
{
	return '0' - c;
}

// -v res/vertex/color.vtx -m res/models/BigBarn/BigBarn.obj
// ..\..\samples\sandbox
int main(int argc, char** argv)
{
	bx::CommandLine cmd(argc, argv);

	const char* vertexLayout = cmd.findOption('v', "vertex");
	if (NULL == vertexLayout)
	{
		logMsg("-v Vertex layout must be provided.");
		return 1;
	}

	const char* modelFile = cmd.findOption('m', "Model");
	if (NULL == modelFile)
	{
		logMsg("-m Model file must be provided.");
		return 1;
	}

	bgfx::VertexDecl vertexDecl;
	std::ifstream vertexFile(vertexLayout, std::ios::in);
	std::string token;
	vertexDecl.begin();
	while (vertexFile >> token)
	{
		if (token == "POSITION")
		{
			vertexDecl.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float);
		}
		else if (token == "NORMAL")
		{
			vertexDecl.add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float);
		}
		else if (token == "TANGENT")
		{
			vertexDecl.add(bgfx::Attrib::Tangent, 3, bgfx::AttribType::Float);
		}
		else if (token == "BITANGENT")
		{
			vertexDecl.add(bgfx::Attrib::Bitangent, 3, bgfx::AttribType::Float);
		}
		else if (!token.rfind("TEXCOORD", 0) && token.length() == 9)
		{
			int index = ctoi(token.back());
			if (index >= 0 && index <= 3)
			{
				vertexDecl.add(static_cast<bgfx::Attrib::Enum>(bgfx::Attrib::TexCoord0 + index), 2, bgfx::AttribType::Float);
			}
			else
			{
				logMsg("Value after TEXCOORD must be in the range 0-3");
				return 1;
			}
		}
		else if (!token.rfind("COLOR", 0) && token.length() == 6)
		{
			int index = ctoi(token.back());
			if (index >= 0 && index <= 3)
			{
				vertexDecl.add(static_cast<bgfx::Attrib::Enum>(bgfx::Attrib::Color0 + index), 4, bgfx::AttribType::Uint8, true);
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
	vertexDecl.end();

	logMsg(std::to_string(vertexDecl.getStride()));

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(modelFile, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices | aiProcess_OptimizeGraph | aiProcess_OptimizeMeshes);
	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		logMsg("Could not open model file");
		return 1;
	}
	
	return 0;
}
