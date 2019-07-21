#include "audio/channel.hpp"

namespace igneous {
Channel::Channel(float volume)
	: volume(volume)
{

}

void Channel::addSource(Source source)
{
	source.setVolume(volume);
	sources.push_back(source);
}

Source Channel::newSource()
{
	Source source;
	source.setVolume(volume);
	sources.push_back(source);
	return source;
}

void Channel::setVolume(float volume)
{
	this->volume = volume;
	for (Source source : sources)
	{
		source.setVolume(volume);
	}
}

Channel::~Channel()
{
}
}
