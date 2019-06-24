#include "ecs/systems/moveSystem.hpp"

#include <GLFW/glfw3.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "ecs/components/cubeComponent.hpp"
#include "ecs/components/transformationComponent.hpp"
#include "core/input.hpp"

namespace MoveSystem
{
	void update(float dt, entt::registry &registry)
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