#include "igneous/audio/source.hpp"

namespace igneous {
Source::Source(glm::vec3 position, glm::vec3 velocity)
	: position(position), velocity(velocity), relative(false), volume(100.0f)
{

}

void Source::play(ALuint buffer)
{
	ALuint id = getFreeSource();
	alSourcei(id, AL_BUFFER, buffer);
	alSourcePlay(id);
}

ALuint Source::getFreeSource()
{
	ALint state;
	for (ALuint id : ids)
	{
		alGetSourcei(id, AL_SOURCE_STATE, &state);
		if (state == AL_STOPPED)
		{
			return id;
		}
	}

	ALuint id;
	alGenSources(1, &id);
	alSource3f(id, AL_POSITION, position.x, position.y, position.z);
	alSourcei(id, AL_SOURCE_RELATIVE, relative);
	alSource3f(id, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
	alSourcef(id, AL_GAIN, volume);
	ids.push_back(id);
	return id;
}

void Source::setPosition(glm::vec3 position)
{
	this->position = position;
	for (ALuint id : ids)
	{
		alSource3f(id, AL_POSITION, position.x, position.y, position.z);
	}
}

void Source::setRelative(bool relative)
{
	this->relative = relative;
	for (ALuint id : ids)
	{
		alSourcei(id, AL_SOURCE_RELATIVE, relative);
	}
}

void Source::setVelocity(glm::vec3 velocity)
{
	this->velocity = velocity;
	for (ALuint id : ids)
	{
		alSource3f(id, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
	}
}

void Source::setVolume(float volume)
{
	this->volume = volume;
	for (ALuint id : ids)
	{
		alSourcef(id, AL_GAIN, volume);
	}
}

Source::~Source()
{
	if (ids.size() > 0)
	{
		alDeleteSources(ids.size(), &ids[0]);
	}
}
}
