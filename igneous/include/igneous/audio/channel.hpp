#pragma once

#include <vector>

#include "igneous/audio/source.hpp"

namespace igneous {
class Channel
{
public:
	Channel(float volume = 100.0f);

	void addSource(Source* source);
	Source* newSource();
	void setVolume(float volume);

	~Channel();
private:
	std::vector<Source*> sources;
	float volume;
};
}
