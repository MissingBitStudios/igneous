#pragma once

#include <deque>

#include <al.h>
#include <alc.h>

#include "../modules/console/Console.h"

class AudioServer
{
public:
	static AudioServer& getInstance();

	const char* getVersion();
	const char* getVendor();
	const char* getRenderer();
	const char* getExtensions();
	const char* getDevices();
	void setListenerData(float x, float y, float z);
	ALuint loadSound(const char * fileName);
	void playSound(ALuint buffer);

	AudioServer(AudioServer const&) = delete;
	void operator=(AudioServer const&) = delete;
private:
	AudioServer();
	~AudioServer();

	static void play_sound_callback(arg_list args);

	const char* ALErrorToString(ALCenum error);

	std::deque<ALuint> buffers;
	std::deque<ALuint> sources;

	ALCdevice* device;
	ALCcontext* context;
};