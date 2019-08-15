#pragma once

#include <filesystem>
#include <string>

bool copyDir(const std::filesystem::path& from, const std::filesystem::path& to);

bool compileModel(const std::filesystem::path& vertexFilePath, const std::filesystem::path& modelFilePath, const std::filesystem::path& outputFilePath);
