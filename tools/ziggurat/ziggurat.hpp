#pragma once

#include <filesystem>
#include <unordered_map>

#define ZIGGURAT_VERSION 0

bool copyDir(const std::filesystem::path& from, const std::filesystem::path& to);
bool copyFile(const std::filesystem::path& from, const std::filesystem::path& to);

bool compileModel(const std::filesystem::path& vertexFilePath, const std::filesystem::path& modelFilePath, const std::filesystem::path& outputFilePath);

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
