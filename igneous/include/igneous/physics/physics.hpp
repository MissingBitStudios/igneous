#pragma once

class Physics
{
public:
	static Physics& getInstance();

	void step(float dt);

	Physics(Physics const&) = delete;
	void operator=(Physics const&) = delete;
};