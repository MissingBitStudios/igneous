#include "renderer/camera.hpp"

#include <bgfx/bgfx.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>

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
	return glm::perspective(glm::radians(60.0f), aspectRatio, 0.1f, 100.0f);
}

glm::mat4 Camera::getRotation() const
{
	glm::quat yaw = glm::angleAxis(rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::quat pitch = glm::angleAxis(rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::quat roll = glm::angleAxis(rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	return glm::mat4(roll * pitch * yaw);
}

void Camera::translateLocal(const glm::vec3& ds)
{
	position += glm::vec3(glm::vec4(ds, 0) * getRotation());
}

void Camera::translateGlobal(const glm::vec3& ds)
{
	position += ds;
}

void Camera::rotateLocal(const glm::vec3& dw)
{
	rotation += dw;
}

const glm::vec3 Camera::forward = glm::vec3(0.0f, 0.0f, -1.0f);
const glm::vec3 Camera::up = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 Camera::left = glm::vec3(-1.0f, 0.0f, 0.0f);