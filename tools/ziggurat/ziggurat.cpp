#include "ziggurat.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>

int main(int argc, char** argv)
{
	if (argc != 4)
	{
		std::cerr << "A config file, source directory, and binary directory must be provided." << std::endl;
		return 1;
	}

	std::string config = argv[1];
	std::filesystem::path sourceDir(argv[2]);
	std::filesystem::path sourceResDir = sourceDir / "res";
	std::filesystem::path binaryDir(argv[3]);
	std::filesystem::path binaryResDir = binaryDir / "res";

	std::ifstream configFile(config, std::ios::in);

	if (configFile.fail())
	{
		std::cerr << "Could not load the config file: " << config << std::endl;
		return 1;
	}

	std::string line;
	while (std::getline(configFile, line))
	{
		if (line.substr(0, 2) != "--")
		{
			size_t firstColon = line.find_first_of(':');
			if (firstColon == std::string::npos)
			{
				std::cerr << "Command name and args required: " << line << std::endl;
				return 1;
			}
			else
			{
				std::string command = line.substr(0, firstColon);
				std::istringstream args(line.substr(firstColon + 1));
				if (command == "MODEL")
				{
					std::string model, vertex;
					if (!(args >> model >> vertex))
					{
						std::cerr << "Both model and vertex required: " << line << std::endl;
						return 1;
					}

					std::filesystem::path vertexPath = sourceResDir / "vertex" / vertex;
					std::filesystem::path modelPath = sourceResDir / "models" / model;
					std::filesystem::path modelBinPath = binaryResDir / "models" / (model.substr(0, model.find_first_of('.')) + ".bin");

					if (!compileModel(vertexPath, modelPath, modelBinPath))
					{
						return 1;
					}
				}
				else if (command == "COPY")
				{
					std::string dir;
					if (!(args >> dir))
					{
						std::cerr << "No dir given for copy: " << line << std::endl;
						return 1;
					}
					if (!copyDir(sourceResDir / dir, binaryResDir / dir))
					{
						return 1;
					}
				}
			}
		}
	}

	return 0;
}
