#include "ecs/systems/skySystem.hpp"

// HDTV rec. 709 matrix.
static float M_XYZ2RGB[] =
{
	 3.240479f, -0.969256f,  0.055648f,
	-1.53715f,   1.875991f, -0.204043f,
	-0.49853f,   0.041556f,  1.057311f,
};

static std::map<float, Color> sunLuminanceXYZTable = {
		{  5.0f, {  0.000000f,  0.000000f,  0.000000f } },
		{  7.0f, { 12.703322f, 12.989393f,  9.100411f } },
		{  8.0f, { 13.202644f, 13.597814f, 11.524929f } },
		{  9.0f, { 13.192974f, 13.597458f, 12.264488f } },
		{ 10.0f, { 13.132943f, 13.535914f, 12.560032f } },
		{ 11.0f, { 13.088722f, 13.489535f, 12.692996f } },
		{ 12.0f, { 13.067827f, 13.467483f, 12.745179f } },
		{ 13.0f, { 13.069653f, 13.469413f, 12.740822f } },
		{ 14.0f, { 13.094319f, 13.495428f, 12.678066f } },
		{ 15.0f, { 13.142133f, 13.545483f, 12.526785f } },
		{ 16.0f, { 13.201734f, 13.606017f, 12.188001f } },
		{ 17.0f, { 13.182774f, 13.572725f, 11.311157f } },
		{ 18.0f, { 12.448635f, 12.672520f,  8.267771f } },
		{ 20.0f, {  0.000000f,  0.000000f,  0.000000f } },
};


// Precomputed luminance of sky in the zenith point in XYZ colorspace.
// Computed using code from Game Engine Gems, Volume One, chapter 15. Implementation based on Dr. Richard Bird model.
// This table is used for piecewise linear interpolation. Day/night transitions are highly inaccurate.
// The scale of luminance change in Day/night transitions is not preserved.
// Luminance at night was increased to eliminate need the of HDR render.
static std::map<float, Color> skyLuminanceXYZTable = {
	{  0.0f, { 0.308f,    0.308f,    0.411f    } },
	{  1.0f, { 0.308f,    0.308f,    0.410f    } },
	{  2.0f, { 0.301f,    0.301f,    0.402f    } },
	{  3.0f, { 0.287f,    0.287f,    0.382f    } },
	{  4.0f, { 0.258f,    0.258f,    0.344f    } },
	{  5.0f, { 0.258f,    0.258f,    0.344f    } },
	{  7.0f, { 0.962851f, 1.000000f, 1.747835f } },
	{  8.0f, { 0.967787f, 1.000000f, 1.776762f } },
	{  9.0f, { 0.970173f, 1.000000f, 1.788413f } },
	{ 10.0f, { 0.971431f, 1.000000f, 1.794102f } },
	{ 11.0f, { 0.972099f, 1.000000f, 1.797096f } },
	{ 12.0f, { 0.972385f, 1.000000f, 1.798389f } },
	{ 13.0f, { 0.972361f, 1.000000f, 1.798278f } },
	{ 14.0f, { 0.972020f, 1.000000f, 1.796740f } },
	{ 15.0f, { 0.971275f, 1.000000f, 1.793407f } },
	{ 16.0f, { 0.969885f, 1.000000f, 1.787078f } },
	{ 17.0f, { 0.967216f, 1.000000f, 1.773758f } },
	{ 18.0f, { 0.961668f, 1.000000f, 1.739891f } },
	{ 20.0f, { 0.264f,    0.264f,    0.352f    } },
	{ 21.0f, { 0.264f,    0.264f,    0.352f    } },
	{ 22.0f, { 0.290f,    0.290f,    0.386f    } },
	{ 23.0f, { 0.303f,    0.303f,    0.404f    } },
};

static Color ABCDE[] =
{
	{ -0.2592f, -0.2608f, -1.4630f },
	{  0.0008f,  0.0092f,  0.4275f },
	{  0.2125f,  0.2102f,  5.3251f },
	{ -0.8989f, -1.6537f, -2.5771f },
	{  0.0452f,  0.0529f,  0.3703f },
};
static Color ABCDE_t[] =
{
	{ -0.0193f, -0.0167f,  0.1787f },
	{ -0.0665f, -0.0950f, -0.3554f },
	{ -0.0004f, -0.0079f, -0.0227f },
	{ -0.0641f, -0.0441f,  0.1206f },
	{ -0.0033f, -0.0109f, -0.0670f },
};

SkySystem::SkySystem()
{
	m_sunLuminanceXYZ.SetMap(sunLuminanceXYZTable);
	m_skyLuminanceXYZ.SetMap(skyLuminanceXYZTable);

	m_time = 0.0f;
	m_timeScale = 1.0f;

	u_sunLuminance = bgfx::createUniform("u_sunLuminance", bgfx::UniformType::Vec4);
	u_skyLuminanceXYZ = bgfx::createUniform("u_skyLuminanceXYZ", bgfx::UniformType::Vec4);
	u_skyLuminance = bgfx::createUniform("u_skyLuminance", bgfx::UniformType::Vec4);
	u_sunDirection = bgfx::createUniform("u_sunDirection", bgfx::UniformType::Vec4);
	u_parameters = bgfx::createUniform("u_parameters", bgfx::UniformType::Vec4);
	u_perezCoeff = bgfx::createUniform("u_perezCoeff", bgfx::UniformType::Vec4, 5);

	m_sky.init(32, 32);

	m_sun.Update(0);

	m_turbidity = 2.15f;
}

void SkySystem::update(float dt)
{
	m_time += m_timeScale * dt;
	m_time = bx::mod(m_time, 24.0f);
	m_sun.Update(m_time);

	Color sunLuminanceXYZ = m_sunLuminanceXYZ.GetValue(m_time);
	Color sunLuminanceRGB = xyzToRgb(sunLuminanceXYZ);

	Color skyLuminanceXYZ = m_skyLuminanceXYZ.GetValue(m_time);
	Color skyLuminanceRGB = xyzToRgb(skyLuminanceXYZ);

	bgfx::setUniform(u_sunLuminance, &sunLuminanceRGB.x);
	bgfx::setUniform(u_skyLuminanceXYZ, &skyLuminanceXYZ.x);
	bgfx::setUniform(u_skyLuminance, &skyLuminanceRGB.x);

	bgfx::setUniform(u_sunDirection, &m_sun.m_sunDir.x);

	float exposition[4] = { 0.02f, 3.0f, 0.1f, m_time };
	bgfx::setUniform(u_parameters, exposition);

	float perezCoeff[4 * 5];
	computePerezCoeff(m_turbidity, perezCoeff);
	bgfx::setUniform(u_perezCoeff, perezCoeff, 5);

	m_sky.draw();

}

Color SkySystem::xyzToRgb(const Color& xyz)
{
	Color rgb;
	rgb.x = M_XYZ2RGB[0] * xyz.x + M_XYZ2RGB[3] * xyz.y + M_XYZ2RGB[6] * xyz.z;
	rgb.y = M_XYZ2RGB[1] * xyz.x + M_XYZ2RGB[4] * xyz.y + M_XYZ2RGB[7] * xyz.z;
	rgb.z = M_XYZ2RGB[2] * xyz.x + M_XYZ2RGB[5] * xyz.y + M_XYZ2RGB[8] * xyz.z;
	return rgb;
};

void SkySystem::computePerezCoeff(float _turbidity, float* _outPerezCoeff)
{
	const bx::Vec3 turbidity = { _turbidity, _turbidity, _turbidity };
	for (uint32_t ii = 0; ii < 5; ++ii)
	{
		const bx::Vec3 tmp = bx::mad(ABCDE_t[ii], turbidity, ABCDE[ii]);
		float* out = _outPerezCoeff + 4 * ii;
		bx::store(out, tmp);
		out[3] = 0.0f;
	}
}

SkySystem::~SkySystem()
{
	m_sky.shutdown();

	bgfx::destroy(u_sunLuminance);
	bgfx::destroy(u_skyLuminanceXYZ);
	bgfx::destroy(u_skyLuminance);
	bgfx::destroy(u_sunDirection);
	bgfx::destroy(u_parameters);
	bgfx::destroy(u_perezCoeff);
}