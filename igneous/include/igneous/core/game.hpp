#pragma once

class Game
{
public:
	virtual void initialize(int _argc, char** _argv) {}
	virtual void update(float dt) {}
	virtual void render() {}
	virtual void shutdown() {}
};
