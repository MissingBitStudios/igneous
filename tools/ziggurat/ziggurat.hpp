#pragma once

#include <filesystem>
#include <unordered_map>
#include <vector>

#define ZIGGURAT_VERSION 1

class StampList
{
public:
	StampList(const std::filesystem::path& stampFilePath);
	~StampList();

	bool isOutOfDate(const std::filesystem::path& file);
private:
	std::unordered_map<std::string, long long> stamps;
	std::filesystem::path dataPath;
};

class Ziggurat
{
public:
	Ziggurat(const std::filesystem::path& sourceDirectory, const std::filesystem::path& binaryDirectory);
	~Ziggurat();

	int build();
private:
	bool copyDir(const std::filesystem::path& path);
	bool copyFile(const std::filesystem::path& path);

	bool createDirs(std::vector<std::string> dirs);

	bool compileModel(const std::string& name);

	bool compileMaterial(const std::string& name);

	std::filesystem::path sourceDir;
	std::filesystem::path sourceResDir;
	std::filesystem::path binaryDir;
	std::filesystem::path binaryResDir;
	StampList* stampList;
};
