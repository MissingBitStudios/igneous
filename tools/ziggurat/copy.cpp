#include "ziggurat.hpp"

#include <filesystem>
#include <iostream>

bool copyDir(const std::filesystem::path& from, const std::filesystem::path& to)
{
	try
	{
		std::filesystem::copy(from, to, std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing);
	}
	catch (const std::filesystem::filesystem_error& e)
	{
		std::cerr << "Copy error on file: " << from << " -> " << e.what() << std::endl;
		return false;
	}
	
	return true;
}

bool copyFile(const std::filesystem::path& from, const std::filesystem::path& to)
{
	try
	{
		std::filesystem::copy_file(from, to, std::filesystem::copy_options::overwrite_existing);
	}
	catch (const std::filesystem::filesystem_error& e)
	{
		std::cerr << "Copy error on file: " << from << " -> " << e.what() << std::endl;
		return false;
	}

	return true;
}
