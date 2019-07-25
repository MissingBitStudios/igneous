// TODO: error checking

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "strings.h"

const std::string readFile(const std::string& filename)
{
	std::ifstream file(filename, std::ios::binary | std::ios::ate);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);
	std::string out;
	out.resize(uint32_t(size + 1));
	if (file.read((char*)out.data(), size))
	{
		return out;
	}
	return "";
}

std::string toHex(unsigned char c)
{
	const char* keys = "0123456789ABCDEF";
	std::string result = "0x";
	result += keys[ c / 16 ];
	result += keys[ c % 16 ];
	return result;
}

int main(int argc, char** argv)
{
	if (argc > 2 && argc % 2 == 0)
	{
		std::vector<std::string> names;
		std::ostringstream out;
		out << header;

		for (int i = 2; i < argc; i += 2)
		{
			std::string file = argv[ i ];
			std::string name = argv[ i + 1 ];
			std::string pureName = name.substr(0, name.find_last_of('_'));
			if (names.size() == 0 || names.back() != pureName)
			{
				names.push_back(pureName);
			}
			std::string contents = readFile(file);
			out << "static const int " + name + "_len = " << contents.length() << ";\n";
			out << "static const unsigned char " + name + "[] = {\n\t";
			for (size_t j = 0; j < contents.length(); ++j)
			{
				out << toHex((unsigned char)contents[ j ]);
				if (j == contents.length() - 1)
				{
					out << "\n";
				}
				else
				{
					out << ",";
					if ((j + 1) % 25 == 0)
					{
						out << "\n\t";
					}
				}
			}
			out << "};\n\n";
		}

		out << footer;

		for (std::string name : names)
		{
			out << "_getShader(" + name + ")\n";
		}

		std::ofstream file(argv[ 1 ], std::ofstream::out);
		file << out.str();
		file.close();
		return 0;
	}
	return -1;
}
