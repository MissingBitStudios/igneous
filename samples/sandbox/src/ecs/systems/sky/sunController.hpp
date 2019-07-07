#pragma once

#include <bx/math.h>

class SunController
{
public:
	enum Month : int
	{
		January = 0,
		February,
		March,
		April,
		May,
		June,
		July,
		August,
		September,
		October,
		November,
		December
	};

	SunController();

	void Update(float _time);

	bx::Vec3 m_northDir;
	bx::Vec3 m_sunDir;
	bx::Vec3 m_upDir;
	float m_latitude;
	Month m_month;

private:
	void CalculateSunOrbit();

	void UpdateSunPosition(float _hour);

	float m_eclipticObliquity;
	float m_delta;
};