#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
	Camera(const glm::vec3& pos = glm::vec3(), const glm::vec3& rot = glm::vec3())
		: position(pos), rotation(rot) {}
	virtual ~Camera() {}

	virtual void update(float dt) {};
	void use(const uint32_t width, const uint32_t height) const;
	glm::mat4 getViewMatrix() const;
	glm::mat4 getProjectionMatrix(const float aspectRatio) const;
	glm::mat4 getRotationMatrix() const;

	const glm::vec3& getPosition() const;
	void translateLocal(const glm::vec3& ds);
	void translateGlobal(const glm::vec3& ds);

	const glm::vec3& getRotation() const;
	void rotateLocal(const glm::vec3& dw);
	void rotateGlobal(const glm::vec3& dw);
protected:
	static const glm::vec3 forward;
	static const glm::vec3 up;
	static const glm::vec3 left;

	glm::vec3 position;
	glm::vec3 rotation;
};
