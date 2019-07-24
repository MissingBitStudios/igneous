#include "igneous/console/conVar.hpp"

#include <istream>
#include <ostream>

namespace igneous {
ConVar::ConVar()
	: callback(nullptr), name("convar"), defaultValue(0.0f), value(0.0f) {};

ConVar::ConVar(const std::string& name, float defaultValue, convar_callback callback)
	: callback(callback), name(name), defaultValue(defaultValue), value(defaultValue) {};

ConVar::ConVar(const std::string& name, float defaultValue, float initialValue, convar_callback callback)
	: callback(callback), name(name), defaultValue(defaultValue), value(initialValue) {};

float ConVar::getDefaultValue() const
{
	return defaultValue;
}

const std::string& ConVar::getName() const
{
	return name;
}

void ConVar::reset()
{
	value = defaultValue;
}

ConVar& ConVar::operator=(bool boolValue)
{
	float newValue = (float)boolValue;
	if (callback)
		callback(value, newValue);
	value = newValue;
	return *this;
}

ConVar& ConVar::operator=(float floatValue)
{
	float newValue = floatValue;
	if (callback)
		callback(value, newValue);
	value = newValue;
	return *this;
}

ConVar& ConVar::operator=(int intValue)
{
	float newValue = (float)intValue;
	if (callback)
		callback(value, newValue);
	value = newValue;
	return *this;
}

ConVar& ConVar::operator=(const std::string& strValue)
{
	float newValue = std::stof(strValue);
	if (callback)
		callback(value, newValue);
	value = newValue;
	return *this;
}

std::ostream& operator<<(std::ostream& os, const ConVar& conVar)
{
	return os << conVar.value;
}

std::istream& operator>>(std::istream& is, ConVar& conVar)
{
	//TODO: When ConVar supports string make this write to the string
	return is;// >> conVar.value;
}

bool ConVar::operator!() const
{
	return !(bool)value;
}

ConVar::operator bool() const
{
	return (bool)value;
}

ConVar::operator float() const
{
	return value;
}

ConVar::operator int() const
{
	return (int)value;
}
} // end namespace igneous
