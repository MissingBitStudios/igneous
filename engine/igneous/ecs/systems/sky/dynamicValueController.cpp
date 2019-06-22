#include "ecs/systems/sky/dynamicValueController.hpp"

void DynamicValueController::SetMap(const KeyMap& keymap)
{
	m_keyMap = keymap;
}

DynamicValueController::ValueType DynamicValueController::GetValue(float time) const
{
	typename KeyMap::const_iterator itUpper = m_keyMap.upper_bound(time + 1e-6f);
	typename KeyMap::const_iterator itLower = itUpper;
	--itLower;

	if (itLower == m_keyMap.end())
	{
		return itUpper->second;
	}

	if (itUpper == m_keyMap.end())
	{
		return itLower->second;
	}

	float lowerTime = itLower->first;
	const ValueType& lowerVal = itLower->second;
	float upperTime = itUpper->first;
	const ValueType& upperVal = itUpper->second;

	if (lowerTime == upperTime)
	{
		return lowerVal;
	}

	return interpolate(lowerTime, lowerVal, upperTime, upperVal, time);
};

void DynamicValueController::Clear()
{
	m_keyMap.clear();
};

DynamicValueController::ValueType DynamicValueController::interpolate(float lowerTime, const ValueType& lowerVal, float upperTime, const ValueType& upperVal, float time) const
{
	const float tt = (time - lowerTime) / (upperTime - lowerTime);
	const ValueType result = bx::lerp(lowerVal, upperVal, tt);
	return result;
};