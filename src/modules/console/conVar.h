#pragma once

#include <iosfwd>
#include <string>

class ConVar
{
public:
	ConVar() {}
	ConVar(const std::string& name, const std::string& defaultValue)
		: name(name), defaultValue(defaultValue), value(defaultValue) {};
	ConVar(const std::string& name, const std::string& defaultValue, const std::string& initialValue)
		: name(name), defaultValue(defaultValue), value(initialValue) {};

	inline const std::string& getDefaultValue() const { return defaultValue; }
	inline const std::string& getName() const { return name; }
	inline void reset() { value = defaultValue; }

	inline ConVar& operator=(bool boolValue) { value = std::to_string(boolValue); return *this; }
	inline ConVar& operator=(float floatValue) { value = std::to_string(floatValue); return *this; }
	inline ConVar& operator=(int intValue) { value = std::to_string(intValue); return *this; }
	inline ConVar& operator=(const std::string& stringValue) { value = stringValue; return *this; }

	inline friend std::ostream& operator<<(std::ostream& os, const ConVar& conVar) { os << conVar.value; return os; }
	inline friend std::istream& operator>>(std::istream& is, ConVar& conVar) { is >> conVar.value; return is; }

	inline bool operator!() const { return (int)*this == 0; }

	inline operator bool() const { return (int)*this != 0; }
	inline operator float() const { try { return std::stof(value); } catch (std::logic_error e) { return 0.0f; } }
	inline operator int() const { try { return std::stoi(value); } catch (std::logic_error e) { return 0; } }
	inline operator std::string() const { return value; }
private:
	std::string defaultValue;
	std::string name;
	std::string value;
};
