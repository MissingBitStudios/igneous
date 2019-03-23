#pragma once

#include <iosfwd>
#include <string>

typedef void(*convar_callback)(std::string, std::string);

class ConVar
{
public:
	ConVar() {}
	ConVar(const std::string& name, const std::string& defaultValue, convar_callback callback = nullptr)
		: callback(callback), defaultValue(defaultValue), name(name), value(defaultValue) {};
	ConVar(const std::string& name, const std::string& defaultValue, const std::string& initialValue, convar_callback callback = nullptr)
		: callback(callback), defaultValue(defaultValue), name(name), value(initialValue) {};

	inline const std::string& getDefaultValue() const { return defaultValue; }
	inline const std::string& getName() const { return name; }
	inline void reset() { value = defaultValue; }

	inline ConVar& operator=(bool boolValue) { std::string newValue = std::to_string(boolValue); if (callback) callback(value, newValue); value = newValue; return *this; }
	inline ConVar& operator=(float floatValue) { std::string newValue = std::to_string(floatValue); if (callback) callback(value, newValue); value = newValue; return *this; }
	inline ConVar& operator=(int intValue) { std::string newValue = std::to_string(intValue); if (callback) callback(value, newValue); value = newValue; return *this; }
	inline ConVar& operator=(const std::string& stringValue) { if (callback) callback(value, stringValue); value = stringValue; return *this; }

	inline friend std::ostream& operator<<(std::ostream& os, const ConVar& conVar) { os << conVar.value; return os; }
	inline friend std::istream& operator>>(std::istream& is, ConVar& conVar) { is >> conVar.value; return is; }

	inline bool operator!() const { return !(bool)*this; }

	inline operator bool() const { try { return std::stoi(value) != 0; } catch (std::logic_error e) { return value != "false"; } }
	inline operator float() const { try { return std::stof(value); } catch (std::logic_error e) { return 0.0f; } }
	inline operator int() const { try { return std::stoi(value); } catch (std::logic_error e) { return 0; } }
	inline operator std::string() const { return value; }
private:
	convar_callback callback;
	std::string defaultValue;
	std::string name;
	std::string value;
};
