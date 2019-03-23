#pragma once

#include <bigg.hpp>
#include <bgfx/bgfx.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Camera
{
public:
	Camera(glm::vec3 pos, const uint32_t width, const uint32_t height)
		: forward(glm::vec3(0.0f, 0.0f, 1.0f)), up(glm::vec3(0.0f, 1.0f, 0.0f)), right(glm::cross(forward, up)),
		view(glm::lookAt(pos, pos + forward, up)), proj(bigg::perspective(glm::radians(60.0f), float(width) / height, 0.1f, 100.0f)) {}
	virtual ~Camera() {}

	virtual void update(float dt) = 0;
	inline void use(const uint32_t width, const uint32_t height)
	{
		bgfx::setViewTransform(0, &view[0][0], &proj[0][0]);
		bgfx::setViewRect(0, 0, 0, uint16_t(width), uint16_t(height));
	}
protected:
	glm::vec3 forward;
	glm::vec3 up;
	glm::vec3 right;
	glm::mat4 view;
	glm::mat4 proj;
};