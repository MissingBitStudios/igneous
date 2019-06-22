#include <igneous.hpp>

class Sandbox : public Game
{
	void initialize(int _argc, char** _argv)
	{
		IG_CLIENT_INFO("initialized");
	}

	void update(float dt)
	{
		IG_CLIENT_INFO("updated");
	}

	void shutdown()
	{
		IG_CLIENT_INFO("shutdown");
	}
};

IG_IMPLEMENT_MAIN(Sandbox)
