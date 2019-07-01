#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "renderer/camera.hpp"

class FPSCamera : public Camera
{
public:
	FPSCamera(const glm::vec3& pos = glm::vec3(), const glm::vec3& rot = glm::vec3())
		: Camera(pos, rot) {}
	~FPSCamera() {}

	void update(const float dt);
	void translateLocal(const glm::vec3& ds);
private:
	double last_x = 0;
	double last_y = 0;
};