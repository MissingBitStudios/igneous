#include "igneous/renderer/camera.hpp"

#include <algorithm>

#include <bgfx/bgfx.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace igneous {
void Camera::use(const uint32_t width, const uint32_t height) const
{
	glm::mat4 view = getViewMatrix();
	glm::mat4 proj = getProjectionMatrix((float)width / (float)height);
	bgfx::setViewTransform(0, &view[0][0], &proj[0][0]);
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::mat4_cast(rotation) * glm::lookAt(position, position + forward, up);
}

glm::mat4 Camera::getProjectionMatrix(float aspectRatio) const
{
	return glm::perspective(glm::radians(60.0f), aspectRatio, 0.1f, 100.0f);
}

const glm::vec3& Camera::getPosition() const
{
	return position;
}

void Camera::translateLocal(const glm::vec3& ds)
{
	position += glm::vec3(glm::vec4(ds, 0) * rotation);
}

void Camera::translateGlobal(const glm::vec3& ds)
{
	position += ds;
}

void Camera::rotateLocal(const glm::vec3& dw)
{
	rotation = vecToQuat(dw) * rotation;
}

void Camera::rotateGlobal(const glm::vec3& dw)
{
	rotation = rotation * vecToQuat(dw);
}

void Camera::setPosition(const glm::vec3& pos)
{
	position = pos;
}

glm::vec3 Camera::getRotation() const
{
	return glm::eulerAngles(rotation);
}

float Camera::getYaw() const
{
	return glm::yaw(rotation);
}

float Camera::getPitch() const
{
	return glm::pitch(rotation);
}

float Camera::getRoll() const
{
	return glm::roll(rotation);
}

void Camera::setRotation(const glm::vec3& rot)
{
	rotation = vecToQuat(rot);
}

void Camera::clampPitch(float min, float max)
{
	//clamp pitch
}

glm::quat Camera::vecToQuat(const glm::vec3& vec)
{
	return glm::quat_cast(glm::eulerAngleZYX(vec.z, vec.y, vec.x));
}

const glm::vec3 Camera::forward = glm::vec3(0.0f, 0.0f, -1.0f);
const glm::vec3 Camera::up = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 Camera::left = glm::vec3(-1.0f, 0.0f, 0.0f);
} // end namespace igneous
