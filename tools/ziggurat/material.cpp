#include "ziggurat.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>
#include <unordered_map>

enum MATERIAL_ATTRIBUTE : uint8_t
{
	COLOR,
	DIFFUSE,
	NORMAL,
	SPECULAR
};

const std::unordered_map<std::string, MATERIAL_ATTRIBUTE> attributeLookupTable = {
	   {"color", COLOR},
	   {"diffuse", DIFFUSE},
	   {"normal", NORMAL},
	   {"specular", SPECULAR}
};

bool Ziggurat::compileMaterial(const std::string& name)
{
	std::filesystem::path materialPath = sourceResDir / "materials" / (name + ".mtl");

	if (!stampList->isOutOfDate(materialPath))
	{
		return true;
	}

	std::ifstream materialFile(materialPath, std::ios::in);

	if (materialFile.fail())
	{
		std::cerr << "Could not load the material file: " << name << std::endl;
		return 1;
	}

	std::ofstream file(binaryResDir / "materials" / (name + ".bin"), std::ios::out | std::ios::binary);

	std::string shaderName;
	materialFile >> shaderName;
	uint8_t shaderNameLength = shaderName.length();
	file.write((char*)&shaderNameLength, sizeof(shaderNameLength));
	file.write(shaderName.c_str(), shaderNameLength);

	std::string attributeName;
	while (materialFile >> attributeName)
	{
		MATERIAL_ATTRIBUTE attribute = attributeLookupTable.at(attributeName);
		file.write((char*)&attribute, sizeof(attribute));

		if (attribute == COLOR)
		{
			std::string r, g, b, a;
			materialFile >> r >> g >> b >> a;
			float vec4[4];
			vec4[0] = std::stof(r);
			vec4[1] = std::stof(g);
			vec4[2] = std::stof(b);
			vec4[3] = std::stof(a);
			file.write((char*)&vec4, sizeof(vec4));
		}
	}

	return true;
}
