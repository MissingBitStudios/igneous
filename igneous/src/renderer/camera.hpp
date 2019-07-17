#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace igneous {
class Camera
{
public:
	Camera(const glm::vec3& pos = glm::vec3(), const glm::vec3& rot = glm::vec3())
		: position(pos), rotation(vecToQuat(rot)) {}
	virtual ~Camera() {}

	virtual void update(float dt) {};
	void use(const uint32_t width, const uint32_t height) const;
	glm::mat4 getViewMatrix() const;
	glm::mat4 getProjectionMatrix(const float aspectRatio) const;

	void translateLocal(const glm::vec3& ds);
	void translateGlobal(const glm::vec3& ds);

	void rotateLocal(const glm::vec3& dw);
	void rotateGlobal(const glm::vec3& dw);

	const glm::vec3& getPosition() const;
	void setPosition(const glm::vec3& pos);

	const glm::vec3& getRotation() const;
	float getYaw() const;
	float getPitch() const;
	float getRoll() const;
	void setRotation(const glm::vec3& rot);

	void clampPitch(float min, float max);

	static glm::quat vecToQuat(const glm::vec3& vec);
protected:
	static const glm::vec3 forward;
	static const glm::vec3 up;
	static const glm::vec3 left;
private:
	glm::vec3 position;
	glm::quat rotation;
};
} // end namespace igneous
