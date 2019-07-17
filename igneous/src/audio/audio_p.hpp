#pragma once

#include <deque>
#include <string>

#include <al.h>
#include <alc.h>

#include "console/console_p.hpp"

namespace igneous {
class Audio
{
public:
	static Audio& getInstance();

	const char* getVersion();
	const char* getVendor();
	const char* getRenderer();
	const char* getExtensions();
	const char* getDevices();
	void setListenerData(float x, float y, float z);
	ALuint loadSound(const char* fileName);
	void playSound(ALuint buffer);

	Audio(Audio const&) = delete;
	void operator=(Audio const&) = delete;
private:
	Audio();
	~Audio();

	static void playSoundCallback(const std::string& name, const arg_list& args);

	const char* ALErrorToString(ALCenum error);

	std::deque<ALuint> buffers;
	std::deque<ALuint> sources;

	ALCdevice* device;
	ALCcontext* context;
};
} // end namespace igneous
