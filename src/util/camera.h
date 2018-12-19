#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Camera
{
public:
	Camera(glm::vec3 pos, const uint32_t width, const uint32_t height);
	~Camera();

	void update(float dt);
	void use(const uint32_t width, const uint32_t height);
private:
	glm::vec3 forward;
	glm::vec3 up;
	glm::vec3 right;
	glm::mat4 view;
	glm::mat4 proj;
};