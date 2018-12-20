#pragma once

#include <vector>

#include "model.h"

class Scene
{
public:
	void update();
	std::vector<Model*> models;
};