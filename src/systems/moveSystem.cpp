#include "moveSystem.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "../components/cameraComponent.h"
#include "../components/cubeComponent.h"
#include "../components/transformationComponent.h"
#include "../util/input.h"

namespace MoveSystem
{
	void update(float dt, entt::registry<> &registry)
	{
		time += dt;

		if (shouldMove)
		{
			registry.view<Transformation, Cube>().each([dt](const auto, auto &transformation, auto &cube)
			{
				glm::mat4 m;
				m[3] = transformation.mtx[3];
				transformation.mtx = m * glm::yawPitchRoll(time + cube.x * 0.21f, time + cube.y * 0.37f, 0.0f);
			});
		}

		registry.view<Transformation, Camera>().each([dt](const auto, auto &transformation, auto &camera)
		{
			glm::vec3 mv;
			if (Input::keys[GLFW_KEY_W]) mv += glm::vec3(0.0f, 0.0f, 5.0f * dt);
			if (Input::keys[GLFW_KEY_A]) mv += glm::vec3(5.0f * dt, 0.0f, 0.0f);
			if (Input::keys[GLFW_KEY_S]) mv += glm::vec3(0.0f, 0.0f, -5.0f * dt);
			if (Input::keys[GLFW_KEY_D]) mv += glm::vec3(-5.0f * dt, 0.0f, 0.0f);
			if (Input::keys[GLFW_KEY_SPACE]) mv += glm::vec3(0.0f, 5.0f * dt, 0.0f);
			if (Input::keys[GLFW_KEY_LEFT_CONTROL]) mv += glm::vec3(0.0f, -5.0f * dt, 0.0f);
			transformation.mtx = glm::translate(transformation.mtx, mv);
		});
	}

	void onKey(int key, int scancode, int action, int mods)
	{
		if (action == GLFW_PRESS)
		{
			switch (key)
			{
			case GLFW_KEY_G:
				shouldMove = !shouldMove;
				break;
			default:
				break;
			}
		}
	}

	float time = 0.0f;
	bool shouldMove = true;
}