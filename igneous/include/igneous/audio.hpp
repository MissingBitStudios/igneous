#pragma once

#include <string>
#include <vector>

#include <al.h>
#include <alc.h>

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
		void setDevice(std::string specifier);
		void setListenerData(float x, float y, float z);
		ALuint loadSound(const char* fileName);
		void playSound(ALuint buffer);

		Audio(Audio const&) = delete;
		void operator=(Audio const&) = delete;
	};
} // end namespace igneous
