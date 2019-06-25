#include "renderer/fpsCamera.hpp"

#include <algorithm>

#include "core/input.hpp"

void FPSCamera::update(const float dt)
{
	float speed = 5.0f;
	if (Input::keys[GLFW_KEY_LEFT_SHIFT]) speed = 10.0f;

	//Calculate change in mouse position
	double dx = Input::mouseX - last_x;
	double dy = Input::mouseY - last_y;

	//Rotate
	glm::vec3 ro;
	ro.x += (float)dy * dt;
	ro.y += (float)dx * dt;

	//Pitch
	if (Input::keys[GLFW_KEY_UP]) ro.x -= speed * dt;
	if (Input::keys[GLFW_KEY_DOWN]) ro.x += speed * dt;
	//Yaw
	if (Input::keys[GLFW_KEY_LEFT]) ro.y -= speed * dt;
	if (Input::keys[GLFW_KEY_RIGHT]) ro.y += speed * dt;
	//Roll
	if (Input::keys[GLFW_KEY_Q]) ro.z -= speed * dt;
	if (Input::keys[GLFW_KEY_E]) ro.z += speed * dt;
	rotateLocal(ro);

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
	translateLocal(mv);

	//Y
	if (Input::keys[GLFW_KEY_SPACE]) mv += up * speed *  dt;
	if (Input::keys[GLFW_KEY_LEFT_CONTROL]) mv -= up * speed *  dt;
	translateGlobal(glm::vec3(0.0f, mv.y, 0.0f));

	last_x = Input::mouseX;
	last_y = Input::mouseY;
}