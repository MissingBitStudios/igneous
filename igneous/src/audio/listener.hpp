#pragma once

#include <al.h>
#include <alc.h>
#include <glm/vec3.hpp>

namespace igneous {
struct ListenerData
{
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 orientation;
};
}
