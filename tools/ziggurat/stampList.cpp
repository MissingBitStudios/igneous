#include "ziggurat.hpp"

#include <fstream>
#include <string>

StampList::StampList(const std::filesystem::path& stampFilePath)
	: dataPath(stampFilePath)
{
	std::ifstream stampFile(dataPath, std::ios::in);

	if (stampFile.good())
	{
		std::string filePath, timestamp;
		while (stampFile >> filePath >> timestamp)
		{
			stamps[filePath] = std::stoll(timestamp);
		}
	}

	if (!stamps.count("ziggurat") || stamps["ziggurat"] < ZIGGURAT_VERSION)
	{
		stamps = { { "ziggurat", ZIGGURAT_VERSION } };
	}
}

StampList::~StampList()
{
	std::ofstream stampFile(dataPath, std::ios::out | std::ios::trunc);

	for (auto it = stamps.begin(); it != stamps.end(); it++)
	{
		stampFile << it->first << " " << it->second << "\n";
	}
}

bool StampList::isOutOfDate(const std::filesystem::path& path)
{
	std::string absolutePath = std::filesystem::absolute(path).string();
	long long current = std::filesystem::last_write_time(path).time_since_epoch().count();
	if (!stamps.count(absolutePath) || stamps.at(absolutePath) < current)
	{
		stamps[absolutePath] = current;
		return true;
	}
	return false;
}
