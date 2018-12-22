#include "camera.h"

#include <bigg.hpp>
#include <bgfx/bgfx.h>

#include "input.h"

Camera::Camera(glm::vec3 pos, const uint32_t width, const uint32_t height)
{
	this->up = glm::vec3(0.0f, 1.0f, 0.0f);
	this->forward = glm::vec3(0.0f, 0.0f, 1.0f);
	this->right = glm::cross(forward, up);
	view = glm::lookAt(pos, pos + forward, up);
	proj = bigg::perspective(glm::radians(60.0f), float(width) / height, 0.1f, 100.0f);
	last_x = Input::mouseX;
	last_y = Input::mouseY;
}

void Camera::update(float dt)
{
	float speed = 5.0f;
	if (Input::keys[GLFW_KEY_LEFT_SHIFT]) speed = 10.0f;
	glm::vec3 mv;
	if (Input::keys[GLFW_KEY_W]) mv += forward * speed * dt;
	if (Input::keys[GLFW_KEY_A]) mv += -right * speed *  dt;
	if (Input::keys[GLFW_KEY_S]) mv += -forward * speed *  dt;
	if (Input::keys[GLFW_KEY_D]) mv += right * speed *  dt;
	if (Input::keys[GLFW_KEY_SPACE]) mv -= up * speed *  dt;
	if (Input::keys[GLFW_KEY_LEFT_CONTROL]) mv -= -up * speed *  dt;
	view = glm::translate(glm::mat4(), mv) * view;

	float dx = Input::mouseX - last_x;
	float dy = Input::mouseY - last_y;
	view = glm::rotate((float) dy * dt, glm::vec3(1.0f, 0.0f, 0.0f)) * view;
	view = glm::rotate((float) dx * dt, glm::vec3(0.0f, 1.0f, 0.0f)) * view;
	if (Input::keys[GLFW_KEY_Q]) view = glm::rotate(glm::radians(-5.0f) * speed *  dt, glm::vec3(0.0f, 0.0f, 1.0f)) * view;
	if (Input::keys[GLFW_KEY_E]) view = glm::rotate(glm::radians(5.0f) * speed *  dt, glm::vec3(0.0f, 0.0f, 1.0f)) * view;
	last_x = Input::mouseX;
	last_y = Input::mouseY;
}

void Camera::use(const uint32_t width, const uint32_t height)
{
	bgfx::setViewTransform(0, &view[0][0], &proj[0][0]);
	bgfx::setViewRect(0, 0, 0, uint16_t(width), uint16_t(height));
}

Camera::~Camera()
{

}