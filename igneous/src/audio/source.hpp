#pragma once

#include <vector>

#include <al.h>
#include <alc.h>
#include <glm/vec3.hpp>

namespace igneous {
class Source
{
public:
	Source(glm::vec3 position = glm::vec3(), glm::vec3 velocity = glm::vec3());

	void play(ALuint buffer);

	void setPosition(glm::vec3 position);
	void setRelative(bool relative);
	void setVelocity(glm::vec3 velocity);
	void setVolume(float volume);

	~Source();
private:
	ALuint getFreeSource();

	std::vector<ALuint> ids;
	glm::vec3 position;
	glm::vec3 velocity;
	bool relative;
	float volume;
};
}
