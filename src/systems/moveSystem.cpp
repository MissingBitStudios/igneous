#include "moveSystem.h"
#include <glm/gtx/euler_angles.hpp> 
#include <GLFW/glfw3.h>

void MoveSystem::update(float dt, entt::registry<> &registry)
{
	if (shouldMove)
	{
		registry.view<Transformation>().each([dt](const auto, auto &transformation)
		{
			glm::vec3 tranform = glm::vec3(transformation.mtx[3]);
			transformation.mtx *= glm::yawPitchRoll(dt + tranform.x * 0.000021f, dt + tranform.y * 0.000037f, 0.0f);
		});
	}
}

void MoveSystem::onKey(int key, int scancode, int action, int mods)
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

bool MoveSystem::shouldMove = true;