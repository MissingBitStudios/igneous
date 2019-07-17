#pragma once

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
		const char* getDevices();
		void setListenerData(float x, float y, float z);
		ALuint loadSound(const char* fileName);
		void playSound(ALuint buffer);

		Audio(Audio const&) = delete;
		void operator=(Audio const&) = delete;
	};
} // end namespace igneous
