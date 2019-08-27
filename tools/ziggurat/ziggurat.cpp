#include "ziggurat.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		std::cerr << "A source directory, and binary directory must be provided." << std::endl;
		return 1;
	}

	Ziggurat ziggurat(argv[1], argv[2]);

	return ziggurat.build();
}

Ziggurat::Ziggurat(const std::filesystem::path& sourceDirectory, const std::filesystem::path& binaryDirectory)
{
	sourceDir = sourceDirectory;
	sourceResDir = sourceDir / "res";

	binaryDir = binaryDirectory;
	binaryResDir = binaryDir / "res";
	stampList = new StampList(binaryDir / ".igneous.stamp");
}

int Ziggurat::build()
{
	createDirs({ "audio", "icons", "textures", "models", "materials" });

	copyDir(sourceResDir / "audio");
	copyDir(sourceResDir / "icons");
	copyDir(sourceResDir / "textures");

	for (auto path : std::filesystem::directory_iterator(sourceResDir / "materials"))
	{
		if (!path.is_directory())
		{
			std::filesystem::path materialPath = path;
			if (materialPath.extension() == ".mtl" && !compileMaterial(materialPath.stem().string()))
			{
				return false;
			}
		}
	}

	for (auto path : std::filesystem::directory_iterator(sourceResDir / "models"))
	{
		if (!path.is_directory())
		{
			std::filesystem::path modelPath = path;
			if (modelPath.extension() == ".mdl" && !compileModel(modelPath.stem().string()))
			{
				return false;
			}
		}
	}

	return 0;
}

Ziggurat::~Ziggurat()
{
	delete stampList;
}
