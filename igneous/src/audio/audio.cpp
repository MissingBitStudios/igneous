#include "audio/audio.hpp"

#define STB_VORBIS_HEADER_ONLY
#include "stb_vorbis.c"

#include "core/log.hpp"

namespace igneous {
Audio::Audio()
{
	IG_CORE_INFO("Initializing Audio");
	// Initialize Open AL quit with error if it fails
	device = alcOpenDevice(NULL);
	if (device)
	{
		context = alcCreateContext(device, NULL); // create context
		if (context)
		{
			if (!alcMakeContextCurrent(context))
			{
				IG_CORE_CRITICAL("OpenAL failed to make context current! Error: {}", ALErrorToString(alGetError()));
			}
		}
		else
		{
			IG_CORE_CRITICAL("OpenAL failed to create a context! Error: {}", ALErrorToString(alGetError()));
		}
	}
	else
	{
		IG_CORE_CRITICAL("OpenAL failed to open a device! Error: {}", ALErrorToString(alGetError()));
	}

	if (alcIsExtensionPresent(NULL, "ALC_enumeration_EXT") == AL_TRUE && alcIsExtensionPresent(NULL, "ALC_enumerate_all_EXT") == AL_TRUE)
	{
		DEVICE_SPECIFIER = ALC_ALL_DEVICES_SPECIFIER;
		DEFAULT_DEVICE_SPECIFIER = ALC_DEFAULT_ALL_DEVICES_SPECIFIER;
	}
	else
	{
		DEVICE_SPECIFIER = ALC_DEVICE_SPECIFIER;
		DEFAULT_DEVICE_SPECIFIER = ALC_DEFAULT_DEVICE_SPECIFIER;
	}

	IG_CORE_INFO("Audio Initialized");

	Console& console = Console::getInstance();
	console.command("play_sound", playSoundCallback);
}

Audio& Audio::getInstance() {
	static Audio instance;
	return instance;
}

const char* Audio::getVersion() { return alGetString(AL_VERSION); }
const char* Audio::getVendor() { return alGetString(AL_VENDOR); }
const char* Audio::getRenderer() { return alGetString(AL_RENDERER); }
const char* Audio::getExtensions() { return alGetString(AL_EXTENSIONS); }

std::vector<std::string> Audio::getDevices()
{
	std::vector<std::string> deviceSpecifiers;
	const ALCchar* devices = alcGetString(NULL, DEVICE_SPECIFIER);
	const ALCchar* device = devices, * next = devices + 1;
	size_t len = 0;

	while (device && *device != '\0' && next && *next != '\0') {
		deviceSpecifiers.push_back(device);
		len = strlen(device);
		device += (len + 1);
		next += (len + 2);
	}

	return deviceSpecifiers;
}

std::string Audio::getDefaultDevice()
{
	return alcGetString(NULL, DEFAULT_DEVICE_SPECIFIER);
}

std::string Audio::getSelectedDevice()
{
	return alcGetString(device, DEVICE_SPECIFIER);
}

void Audio::setDevice(std::string specifier)
{
	device = alcOpenDevice(specifier.c_str());
	if (!device)
	{
		IG_CORE_CRITICAL("OpenAL failed to open a device! Error: {}", ALErrorToString(alGetError()));
	}
}

void Audio::setListenerData(float x, float y, float z) {
	alListener3f(AL_POSITION, x, y, z);
	alListener3f(AL_VELOCITY, 0, 0, 0);
}

ALuint Audio::loadSound(const char* fileName) {
	ALuint buffer;
	alGenBuffers(1, &buffer);
	buffers.push_back(buffer);
	int channels;
	int sampleRate;
	ALshort* output;
	int len = stb_vorbis_decode_filename(fileName, &channels, &sampleRate, &output);
	int width = sizeof(ALshort);
	ALenum format = 0;
	if (width == 1)
	{
		if (channels == 1)
			format = AL_FORMAT_MONO8;
		else if (channels == 2)
			format = AL_FORMAT_STEREO8;
	}
	else if (width == 2)
	{
		if (channels == 1)
			format = AL_FORMAT_MONO16;
		else if (channels == 2)
			format = AL_FORMAT_STEREO16;
	}
	ALsizei size = len * channels * width;
	alBufferData(buffer, format, output, size, sampleRate);
	return buffer;
}

void Audio::playSound(ALuint buffer)
{
	ALuint source;
	alGenSources(1, &source);
	const ALuint buffers[1] = { buffer };
	alSourceQueueBuffers(source, 1, buffers);
	alSourcePlay(source);
}

void Audio::playSoundCallback(const std::string& name, const arg_list& args)
{
	if (args.size() > 0)
	{
		Audio& audio = getInstance();
		std::string path = "res/audio/" + args[0] + ".ogg";
		audio.playSound(audio.loadSound(path.c_str()));
	}
}

const char* Audio::ALErrorToString(ALCenum error)
{
	switch (error)
	{
	case ALC_NO_ERROR: return "ALC_NO_ERROR";
	case ALC_INVALID_DEVICE: return "ALC_INVALID_DEVICE";
	case ALC_INVALID_CONTEXT: return "ALC_INVALID_CONTEXT";
	case ALC_INVALID_ENUM: return "ALC_INVALID_ENUM";
	case ALC_INVALID_VALUE: return "ALC_INVALID_VALUE";
	case ALC_OUT_OF_MEMORY: return "ALC_OUT_OF_MEMORY";
	default: break;
	}
	return "Failed to find error";
}

Audio::~Audio()
{
	IG_CORE_INFO("Shutingdown Audio");
	for (const ALuint buffer : buffers) {
		alDeleteBuffers(1, &buffer);
		buffers.pop_front();
	}

	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
	IG_CORE_INFO("Audio Shutdown");
}
} // end namespace igneous
