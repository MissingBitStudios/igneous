#pragma once

#include <string>
#include <vector>

#include <al.h>
#include <alc.h>
#include <glm/vec3.hpp>

namespace igneous {
class Source
{
public:
	Source(glm::vec3 position = glm::vec3(), glm::vec3 velocity = glm::vec3(), glm::vec3 orientation = glm::vec3());

	void play(ALuint buffer);

	void setOrientation(glm::vec3 orientation);
	void setPosition(glm::vec3 position);
	void setRelative(bool relative);
	void setVelocity(glm::vec3 velocity);
	void setVolume(float volume);

	~Source();
};

class Channel
{
public:
	Channel(float volume = 100.0f);

	void addSource(Source source);
	Source newSource();
	void setVolume(float volume);

	~Channel();
private:
	std::vector<Source> sources;
	float volume;
};

class Audio
{
public:
	static Audio& getInstance();

	const char* getVersion();
	const char* getVendor();
	const char* getRenderer();
	const char* getExtensions();
	std::vector<std::string> getDevices();
	std::string getDefaultDevice();
	std::string getSelectedDevice();
	Source* getAmbientSource();
	void setDevice(const std::string& specifier);
	void setListenerData(float x, float y, float z);
	ALuint loadSound(const std::string& fileName);

	Audio(Audio const&) = delete;
	void operator=(Audio const&) = delete;
};
} // end namespace igneous
