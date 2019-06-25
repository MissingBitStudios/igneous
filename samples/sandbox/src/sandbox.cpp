#include <igneous.hpp>

class Sandbox : public Game
{
	void initialize(int _argc, char** _argv)
	{
		Renderer& renderer = Renderer::getInstance();

		Vertex::init();

		sky = new SkySystem;

		polyShader = renderer.loadProgram("vs_cubes", "fs_cubes");

		barn = new Model("res/models/BigBarn/BigBarn.obj");

		handle = renderer.loadTexture("res/icons/icon48.png", false);

		auto entity = registry.create();
		registry.assign<ModelComponent>(entity, barn, polyShader);
		registry.assign<Transformation>(entity, glm::mat4());

		camera = new FPSCamera();
	}

	void update(float dt)
	{
		camera->update(dt);
		sky->update(dt);
	}

	void render()
	{
		camera->use(Input::width, Input::height);
		RendererSystem::render(registry, handle);
	}

	void shutdown()
	{
		bgfx::destroy(handle);
		bgfx::destroy(polyShader);
		delete barn;
		delete sky;
		delete camera;
	}
private:
	entt::registry registry;
	bgfx::ProgramHandle polyShader;
	bgfx::TextureHandle handle;
	Model* barn;
	SkySystem* sky;
	Camera* camera;
};

IG_IMPLEMENT_MAIN(Sandbox)
