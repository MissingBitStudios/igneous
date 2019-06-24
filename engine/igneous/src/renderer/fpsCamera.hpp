#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "renderer/camera.hpp"
#include "core/input.hpp"

class FPSCamera : public Camera
{
public:
	FPSCamera(glm::vec3 pos, const uint32_t width, const uint32_t height)
		: Camera(pos, width, height), last_x(Input::mouseX), last_y(Input::mouseY), pitch(0), yaw(-1.570796326794897), pos(pos)
	{
		if (pitch > 6.283185307179586)
		{
			do
			{
				pitch -= 6.283185307179586;
			} while (pitch > 6.283185307179586);
		}
		else if (pitch < 0)
		{
			do
			{
				pitch += 6.283185307179586;
			} while (pitch < 0);
		}
		if (pitch > 6.283185307179586)
		{
			do
			{
				pitch -= 6.283185307179586;
			} while (pitch > 6.283185307179586);
		}
		else if (yaw < 0)
		{
			do
			{
				yaw += 6.283185307179586;
			} while (yaw < 0);
		}
	}
	~FPSCamera() {}

	void update(float dt);
private:
	double last_x;
	double last_y;
	double pitch;
	double yaw;
	glm::vec3 pos;
};