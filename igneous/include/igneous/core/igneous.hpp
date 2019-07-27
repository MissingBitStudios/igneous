#pragma once

#include <bgfx/bgfx.h>
#include <GLFW/glfw3.h>

#include <bx/allocator.h>

namespace igneous
{
// allocator
class Allocator : public bx::AllocatorI
{
public:
	void* realloc(void* _ptr, size_t _size, size_t _align, const char* _file, uint32_t _line)
	{
		if (_size == 0)
		{
			free(_ptr);
			return nullptr;
		}
		else
		{
			return malloc(_size);
		}
	}
};

// Application
class Application
{
public:
	Application(const char* title = "", uint32_t width = 1280, uint32_t height = 720);
	virtual ~Application() {};

	int run(int argc, char** argv, bgfx::Init init);
	int run(
		int argc,
		char** argv,
		bgfx::RendererType::Enum type = bgfx::RendererType::Count,
		uint16_t vendorId = BGFX_PCI_ID_NONE,
		uint16_t deviceId = 0,
		bgfx::CallbackI * callback = NULL,
		bx::AllocatorI * allocator = NULL
	);

	virtual void initialize(int _argc, char** _argv) {};
	virtual void update(float dt) {};
	virtual void render() {};
	virtual int shutdown() { return 0; };

	virtual void onReset();
	virtual void onKey(int key, int scancode, int action, int mods) {}
	virtual void onChar(unsigned int codepoint) {}
	virtual void onCharMods(int codepoint, unsigned int mods) {}
	virtual void onMouseButton(int button, int action, int mods) {}
	virtual void onCursorPos(double xpos, double ypos) {}
	virtual void onCursorEnter(int entered) {}
	virtual void onScroll(double xoffset, double yoffset) {}
	virtual void onDrop(int count, const char** paths) {}
	virtual void onWindowSize(int width, int height) {}
};
} // end namespace igneous

#define IG_IMPLEMENT_MAIN(APPLICATION_CLASS) \
int main(int argc, char** argv)              \
{                                            \
	APPLICATION_CLASS app;                   \
	return app.run(argc, argv);              \
}
