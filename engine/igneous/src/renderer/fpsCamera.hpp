#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "renderer/camera.hpp"
#include "core/input.hpp"

class FPSCamera : public Camera
{
public:
	FPSCamera(const glm::vec3& pos = glm::vec3(), const glm::vec3& rot = glm::vec3())
		: Camera(pos, rot) {}
	~FPSCamera() {}

	void update(const float dt);
};