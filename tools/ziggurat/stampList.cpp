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
}

StampList::~StampList()
{
	std::ofstream stampFile(dataPath, std::ios::out | std::ofstream::trunc);

	for (auto it = stamps.begin(); it != stamps.end(); it++)
	{
		stampFile << it->first << " " << it->second << "\n";
	}
}

bool StampList::isOutOfDate(const std::filesystem::path& file)
{
	long long current = std::filesystem::last_write_time(file).time_since_epoch().count();
	if (!stamps.count(file.string()) || stamps.at(file.string()) < current)
	{
		stamps[file.string()] = current;
		return true;
	}
	return false;
}
