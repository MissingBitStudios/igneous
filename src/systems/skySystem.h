#pragma once

#include <bgfx/bgfx.h>

#include "sky/dynamicValueController.h"
#include "sky/proceduralSky.h"
#include "sky/sunController.h"

class SkySystem
{
public:
	SkySystem();
	~SkySystem();

	void update(float dt);

	SkySystem(SkySystem const&) = delete;
	void operator=(SkySystem const&) = delete;
private:
	Color xyzToRgb(const Color& xyz);
	void computePerezCoeff(float _turbidity, float* _outPerezCoeff);

	bgfx::UniformHandle u_sunLuminance;
	bgfx::UniformHandle u_skyLuminanceXYZ;
	bgfx::UniformHandle u_skyLuminance;
	bgfx::UniformHandle u_sunDirection;
	bgfx::UniformHandle u_parameters;
	bgfx::UniformHandle u_perezCoeff;

	ProceduralSky m_sky;
	SunController m_sun;

	DynamicValueController m_sunLuminanceXYZ;
	DynamicValueController m_skyLuminanceXYZ;

	float m_time;
	float m_timeScale;

	float m_turbidity;
};