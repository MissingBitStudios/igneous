#include "renderer/fpsCamera.hpp"

void FPSCamera::update(float dt)
{
	double dx = Input::mouseX - last_x;
	double dy = Input::mouseY - last_y;
	pitch -= 0.2 * dy * dt;
	yaw += 0.2 * dx * dt;

	// keeps the camera from turning upside down
	if (pitch > 1.5)
		pitch = 1.5;
	if (pitch < -1.5)
		pitch = -1.5;

	// create the rotation matrix
	glm::mat4 rot = glm::rotate((float)pitch, right);
	rot = glm::rotate(rot, (float)yaw, up);

	// changes the position vector
	float speed = 5.0f;
	if (Input::keys[GLFW_KEY_LEFT_SHIFT]) speed = 10.0f;
	glm::vec3 mv;
	if (Input::keys[GLFW_KEY_W]) mv += forward * speed * dt;
	if (Input::keys[GLFW_KEY_A]) mv += -right * speed *  dt;
	if (Input::keys[GLFW_KEY_S]) mv += -forward * speed *  dt;
	if (Input::keys[GLFW_KEY_D]) mv += right * speed *  dt;
	if (Input::keys[GLFW_KEY_SPACE]) mv -= up * speed *  dt;
	if (Input::keys[GLFW_KEY_LEFT_CONTROL]) mv -= -up * speed *  dt;
	glm::vec4 dv(mv, 0);
	dv = dv * rot;
	pos += glm::vec3(dv);

	// applies the translation and rotation to the view matrix
	view = rot * glm::translate(glm::mat4(), pos) * glm::lookAt(glm::vec3(), glm::vec3() + forward, up);

	last_x = Input::mouseX;
	last_y = Input::mouseY;
}