#pragma once

#include <map>

#include <bx/math.h>

typedef bx::Vec3 Color;

class DynamicValueController
{
	typedef Color ValueType;
	typedef std::map<float, ValueType> KeyMap;

public:
	DynamicValueController() {}
	~DynamicValueController() {}

	void SetMap(const KeyMap& keymap);

	ValueType GetValue(float time) const;

	void Clear();

private:
	ValueType interpolate(float lowerTime, const ValueType& lowerVal, float upperTime, const ValueType& upperVal, float time) const;

	KeyMap	m_keyMap;
};