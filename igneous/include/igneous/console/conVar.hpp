#pragma once

#include <iosfwd>
#include <string>

namespace igneous {
typedef void(*convar_callback)(float, float);

class ConVar
{
public:
	ConVar();
	ConVar(const std::string& name, float defaultValue, convar_callback callback = nullptr);
	ConVar(const std::string& name, float defaultValue, float initialValue, convar_callback callback = nullptr);
	ConVar(const ConVar&) = delete;

	float getDefaultValue() const;
	const std::string& getName() const;
	void reset();

	ConVar& operator=(bool boolValue);
	ConVar& operator=(float floatValue);
	ConVar& operator=(int intValue);
	ConVar& operator=(const std::string& strValue);

	friend std::ostream& operator<<(std::ostream& os, const ConVar& conVar);
	friend std::istream& operator>>(std::istream& is, ConVar& conVar);

	bool operator!() const;

	operator bool() const;
	operator float() const;
	operator int() const;
};
} // end namespace igneous
