#include "ziggurat.hpp"

bool Ziggurat::createDirs(std::vector<std::string> dirs)
{
	for (std::string dir : dirs)
	{
		std::filesystem::create_directories(binaryResDir / dir);
	}
	return true;
}
