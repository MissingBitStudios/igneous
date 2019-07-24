#pragma once

#include <map>
#include <string>
#include <vector>

#include <al.h>
#include <alc.h>

#include "igneous/audio/source.hpp"
#include "igneous/audio/listener.hpp"
#include "igneous/console/console.hpp"

namespace igneous {
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
	void setListener(const ListenerData& listener);
	ALuint loadSound(const std::string& fileName);

	Audio(Audio const&) = delete;
	void operator=(Audio const&) = delete;
private:
	Audio();
	~Audio();

	static void playSoundCallback(const std::string& name, const arg_list& args);

	const char* ALErrorToString(ALCenum error);

	std::map<std::string, ALuint> buffers;

	ALCdevice* device;
	ALCcontext* context;

	Source* ambient;

	ALCenum DEVICE_SPECIFIER, DEFAULT_DEVICE_SPECIFIER;
};
} // end namespace igneous
