#include "renderer/camera.hpp"

#include <bigg.hpp>

void Camera::use(const uint32_t width, const uint32_t height)
{
	glm::mat4 view = getView();
	glm::mat4 proj = getProjection((float)width / (float)height);
	bgfx::setViewTransform(0, &view[0][0], &proj[0][0]);
}

glm::mat4 Camera::getView() const
{
	return getRotation() * glm::lookAt(position, position + forward, up);
}

glm::mat4 Camera::getProjection(const float aspectRatio) const
{
	return bigg::perspective(glm::radians(60.0f), aspectRatio, 0.1f, 100.0f);
}

glm::mat4 Camera::getRotation() const
{
	glm::mat4 rot = glm::rotate(-rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	rot = glm::rotate(rot, -rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	rot = glm::rotate(rot, -rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	return rot;
}

void Camera::translate(const glm::vec3& ds)
{
	position += glm::vec3(glm::vec4(ds, 0) * getRotation());
}

void Camera::rotate(const glm::vec3& dw)
{
	rotation += dw;
}

const glm::vec3 Camera::forward = glm::vec3(0.0f, 0.0f, -1.0f);
const glm::vec3 Camera::up = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 Camera::left = glm::vec3(-1.0f, 0.0f, 0.0f);