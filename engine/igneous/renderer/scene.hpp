#pragma once

#include <vector>

#include "renderer/model.hpp"

class Scene
{
public:
	void update();
	std::vector<Model*> models;
};