#include "ziggurat.hpp"

#include <filesystem>
#include <iostream>

bool Ziggurat::copyDir(const std::filesystem::path& path)
{
	for (auto path : std::filesystem::recursive_directory_iterator(path))
	{
		if (!path.is_directory())
		{
			if (!copyFile(path))
			{
				return false;
			}
		}
		else
		{
			std::filesystem::path to = binaryDir / std::filesystem::relative(path, sourceDir);
			std::filesystem::create_directories(to);
		}
	}
	return true;
}

bool Ziggurat::copyFile(const std::filesystem::path& path)
{
	if (!stampList->isOutOfDate(path))
	{
		return true;
	}

	try
	{
		std::filesystem::path to = binaryDir / std::filesystem::relative(path, sourceDir);
		std::filesystem::copy_file(path, to, std::filesystem::copy_options::overwrite_existing);
	}
	catch (const std::filesystem::filesystem_error& e)
	{
		std::cerr << "Copy error on file: " << path << " -> " << e.what() << std::endl;
		return false;
	}

	return true;
}
