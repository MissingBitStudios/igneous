#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "camera.h"
#include "input.h"

class FPSCamera : public Camera
{
public:
	FPSCamera(glm::vec3 pos, const uint32_t width, const uint32_t height)
		: Camera(pos, width, height), last_x(Input::mouseX), last_y(Input::mouseY) {}
	~FPSCamera() {}

	void update(float dt);
private:
	double last_x;
	double last_y;
};