#include "moveSystem.h"
#include <glm/gtx/euler_angles.hpp> 
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "../components/cameraComponent.h"
#include "../components/cubeComponent.h"

void MoveSystem::update(float dt, entt::registry<> &registry)
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

		switch (dir)
		{
		case 1:
			mv = glm::vec3(0.0f, 0.0f, 5.0f * dt);
			break;
		case 2:
			mv = glm::vec3(5.0f * dt, 0.0f, 0.0f);
			break;
		case 3:
			mv = glm::vec3(0.0f, 0.0f, -5.0f * dt);
			break;
		case 4:
			mv = glm::vec3(-5.0f * dt, 0.0f, 0.0f);
			break;
		case 5:
			mv = glm::vec3(0.0f, 5.0f * dt, 0.0f);
			break;
		case 6:
			mv = glm::vec3(0.0f, -5.0f * dt, 0.0f);
			break;
		default:
			break;
		}
		transformation.mtx = glm::translate(transformation.mtx, mv);
	});
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

	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_W:
			dir = 1;
			break;
		case GLFW_KEY_A:
			dir = 2;
			break;
		case GLFW_KEY_S:
			dir = 3;
			break;
		case GLFW_KEY_D:
			dir = 4;
			break;
		case GLFW_KEY_SPACE:
			dir = 5;
			break;
		case GLFW_KEY_LEFT_CONTROL:
			dir = 6;
			break;
		default:
			break;
		}
	}
	else if(action == GLFW_RELEASE)
	{
		dir = 0;
	}
}

float MoveSystem::time = 0.0f;
int MoveSystem::dir = 0;
bool MoveSystem::shouldMove = true;