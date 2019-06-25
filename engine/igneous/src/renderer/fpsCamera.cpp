#include "renderer/fpsCamera.hpp"

#include <algorithm>

void FPSCamera::update(const float dt)
{
	float speed = 5.0f;
	if (Input::keys[GLFW_KEY_LEFT_SHIFT]) speed = 10.0f;

	//Rotate
	glm::vec3 ro;
	//Pitch
	if (Input::keys[GLFW_KEY_UP]) ro.x += speed * dt;
	if (Input::keys[GLFW_KEY_DOWN]) ro.x -= speed * dt;
	//Yaw
	if (Input::keys[GLFW_KEY_LEFT]) ro.y += speed * dt;
	if (Input::keys[GLFW_KEY_RIGHT]) ro.y -= speed * dt;
	//Roll
	if (Input::keys[GLFW_KEY_Q]) ro.z += speed * dt;
	if (Input::keys[GLFW_KEY_E]) ro.z -= speed * dt;
	rotate(ro);

	//Clamp pitch
	rotation.x = std::clamp(rotation.x, glm::radians(-90.0f), glm::radians(90.0f));

	//Translate
	glm::vec3 mv;
	//Z
	if (Input::keys[GLFW_KEY_W]) mv += forward * speed * dt;
	if (Input::keys[GLFW_KEY_S]) mv -= forward * speed * dt;
	//X
	if (Input::keys[GLFW_KEY_A]) mv += left * speed *  dt;
	if (Input::keys[GLFW_KEY_D]) mv -= left * speed *  dt;
	//Y
	if (Input::keys[GLFW_KEY_SPACE]) mv += up * speed *  dt;
	if (Input::keys[GLFW_KEY_LEFT_CONTROL]) mv -= up * speed *  dt;
	translate(mv);
}