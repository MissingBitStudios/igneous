#include "renderer/camera.hpp"

#include <bgfx/bgfx.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>

void Camera::use(const uint32_t width, const uint32_t height) const
{
	glm::mat4 view = getViewMatrix();
	glm::mat4 proj = getProjectionMatrix((float)width / (float)height);
	bgfx::setViewTransform(0, &view[0][0], &proj[0][0]);
}

glm::mat4 Camera::getViewMatrix() const
{
	return getRotationMatrix() * glm::lookAt(position, position + forward, up);
}

glm::mat4 Camera::getProjectionMatrix(float aspectRatio) const
{
	return glm::perspective(glm::radians(60.0f), aspectRatio, 0.1f, 100.0f);
}

glm::mat4 Camera::getRotationMatrix() const
{
	return glm::eulerAngleXYZ(rotation.x, rotation.y, rotation.z);
}

const glm::vec3& Camera::getPosition() const
{
	return position;
}

void Camera::translateLocal(const glm::vec3& ds)
{
	position += glm::vec3(glm::vec4(ds, 0) * getRotationMatrix());
}

void Camera::translateGlobal(const glm::vec3& ds)
{
	position += ds;
}

const glm::vec3& Camera::getRotation() const
{
	return rotation;
}

void Camera::rotateLocal(const glm::vec3& dw)
{
	//TODO: rotate relative to current rotation
	rotation += dw;
}

void Camera::rotateGlobal(const glm::vec3& dw)
{
	//TODO: rotate relative to world grid
	rotation += dw;
}

const glm::vec3 Camera::forward = glm::vec3(0.0f, 0.0f, -1.0f);
const glm::vec3 Camera::up = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 Camera::left = glm::vec3(-1.0f, 0.0f, 0.0f);