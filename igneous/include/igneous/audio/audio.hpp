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
namespace audio
{
	void init();
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
	void shutdown();
}
} // end namespace igneous
