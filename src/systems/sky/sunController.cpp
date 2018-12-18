#include "sunController.h"

SunController::SunController()
	: m_latitude(50.0f)
	, m_month(June)
	, m_eclipticObliquity(bx::toRad(23.4f))
	, m_delta(0.0f)
{
	m_northDir = { 1.0f,  0.0f, 0.0f };
	m_sunDir = { 0.0f, -1.0f, 0.0f };
	m_upDir = { 0.0f,  1.0f, 0.0f };
}

void SunController::Update(float _time)
{
	CalculateSunOrbit();
	UpdateSunPosition(_time - 12.0f);
}

void SunController::CalculateSunOrbit()
{
	float day = 30.0f * m_month + 15.0f;
	float lambda = 280.46f + 0.9856474f * day;
	lambda = bx::toRad(lambda);
	m_delta = bx::asin(bx::sin(m_eclipticObliquity) * bx::sin(lambda));
}

void SunController::UpdateSunPosition(float _hour)
{
	const float latitude = bx::toRad(m_latitude);
	const float hh = _hour * bx::kPi / 12.0f;
	const float azimuth = bx::atan2(
		bx::sin(hh)
		, bx::cos(hh) * bx::sin(latitude) - bx::tan(m_delta) * bx::cos(latitude)
	);

	const float altitude = bx::asin(
		bx::sin(latitude) * bx::sin(m_delta) + bx::cos(latitude) * bx::cos(m_delta) * bx::cos(hh)
	);

	const bx::Quaternion rot0 = bx::rotateAxis(m_upDir, -azimuth);
	const bx::Vec3 dir = bx::mul(m_northDir, rot0);
	const bx::Vec3 uxd = bx::cross(m_upDir, dir);

	const bx::Quaternion rot1 = bx::rotateAxis(uxd, altitude);
	m_sunDir = bx::mul(dir, rot1);
}