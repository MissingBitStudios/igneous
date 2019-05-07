#include "audioServer.h"

#define STB_VORBIS_HEADER_ONLY
#include "stb_vorbis.c"

#include "../util/log.h"

AudioServer::AudioServer()
{
	IG_CORE_INFO("Initializing Audio Server");
	// Initialize Open AL quit with error if it fails
	const ALCchar* deviceName = alcGetString(0, ALC_DEVICE_SPECIFIER);
	device = alcOpenDevice(deviceName);
	if (device)
	{
		context = alcCreateContext(device, NULL); // create context
		if (context)
		{
			if (!alcMakeContextCurrent(context))
			{
				IG_CORE_ERROR("OpenAL failed to make context current! Error: {}", ALErrorToString(alGetError()));
			}
		}
		else
		{
			IG_CORE_ERROR("OpenAL failed to create a context! Error: {}", ALErrorToString(alGetError()));
		}
	}
	else
	{
		IG_CORE_ERROR("OpenAL failed to open a device! Error: {}", ALErrorToString(alGetError()));
	}
	IG_CORE_INFO("Audio Server Initialized");

	Console& console = Console::getInstance();
	console.command("play_sound", playSoundCallback);
}

AudioServer& AudioServer::getInstance() {
	static AudioServer instance;
	return instance;
}

const char* AudioServer::getVersion() { return alGetString(AL_VERSION); }
const char* AudioServer::getVendor() { return alGetString(AL_VENDOR); }
const char* AudioServer::getRenderer() { return alGetString(AL_RENDERER); }
const char* AudioServer::getExtensions() { return alGetString(AL_EXTENSIONS); }
const char* AudioServer::getDevices() { return alcGetString(NULL, ALC_DEVICE_SPECIFIER); }

void AudioServer::setListenerData(float x, float y, float z) {
	alListener3f(AL_POSITION, x, y, z);
	alListener3f(AL_VELOCITY, 0, 0, 0);
}

ALuint AudioServer::loadSound(const char* fileName) {
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

void AudioServer::playSound(ALuint buffer)
{
	ALuint source;
	alGenSources(1, &source);
	const ALuint buffers[1] = { buffer };
	alSourceQueueBuffers(source, 1, buffers);
	alSourcePlay(source);
}

void AudioServer::playSoundCallback(const std::string& name, const arg_list& args)
{
	if (args.size() > 0)
	{
		AudioServer& audio = getInstance();
		std::string path = "res/audio/" + args[0] + ".ogg";
		audio.playSound(audio.loadSound(path.c_str()));
	}
}

const char* AudioServer::ALErrorToString(ALCenum error)
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

AudioServer::~AudioServer()
{
	IG_CORE_INFO("Shutingdown Audio Server");
	for (const ALuint buffer : buffers) {
		alDeleteBuffers(1, &buffer);
		buffers.pop_front();
	}

	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
	IG_CORE_INFO("Audio Server Shutdown");
}