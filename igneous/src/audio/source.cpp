#include "audio/source.hpp"

namespace igneous {
Source::Source(glm::vec3 position, glm::vec3 velocity, glm::vec3 orientation)
{
	alGenSources(1, &id);
	setOrientation(orientation);
	setPosition(position);
	setVelocity(velocity);
}

void Source::play(ALuint buffer)
{
	alSourcei(id, AL_BUFFER, buffer);
	alSourcePlay(id);
}

void Source::setOrientation(glm::vec3 orientation)
{
	alSource3f(id, AL_ORIENTATION, orientation.x, orientation.y, orientation.z);
}

void Source::setPosition(glm::vec3 position)
{
	alSource3f(id, AL_POSITION, position.x, position.y, position.z);
}

void Source::setRelative(bool relative)
{
	alSourcei(id, AL_SOURCE_RELATIVE, relative);
}

void Source::setVelocity(glm::vec3 velocity)
{
	alSource3f(id, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
}

void Source::setVolume(float volume)
{
	alSourcef(id, AL_GAIN, volume);
}

Source::~Source()
{
	alDeleteSources(1, &id);
}
}
