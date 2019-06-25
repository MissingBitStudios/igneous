#pragma once

#include <bgfx/bgfx.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Camera
{
public:
	Camera(const glm::vec3& pos = glm::vec3(), const glm::vec3& rot = glm::vec3())
		: position(pos), rotation(rot) {}
	virtual ~Camera() {}

	virtual void update(const float dt) {};
	void use(const uint32_t width, const uint32_t height);
	glm::mat4 getView() const;
	glm::mat4 getProjection(const float aspectRatio) const;
	glm::mat4 getRotation() const;

	void translate(const glm::vec3& ds);
	void rotate(const glm::vec3& dw);
protected:
	static const glm::vec3 forward;
	static const glm::vec3 up;
	static const glm::vec3 left;

	glm::vec3 position;
	glm::vec3 rotation;
};
