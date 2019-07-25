#include <igneous/igneous.hpp>

#include <entt/entt.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ecs/systems/skySystem.hpp"

using namespace igneous;

class Sandbox : public Application
{
public:
	Sandbox()
		: Application("Sandbox")
	{
	
	}

	void initialize(int _argc, char** _argv)
	{
		Renderer& renderer = Renderer::getInstance();

		gui::setTheme(gui::Theme::CHERRY);

		sky = new SkySystem;

		polyShader = renderer.loadProgram("vs_cubes", "fs_cubes");

		barn = renderer.loadModel<GenericVertex>("res/models/BigBarn/BigBarn.obj");

		auto entity = registry.create();
		registry.assign<ModelComponent>(entity, barn, polyShader);
		registry.assign<Transformation>(entity, glm::identity<glm::mat4>());

		camera = new FPSCamera(glm::vec3(0.0f, 5.0f, 15.0f));
	}

	void update(float dt)
	{
		camera->update(dt);
		sky->update(dt);
	}

	void onReset()
	{
		bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
		bgfx::setViewRect(0, 0, 0, uint16_t(getWidth()), uint16_t(getHeight()));
	}

	void render()
	{
		camera->use(getWidth(), getHeight());
		RendererSystem::render(registry);

		ImGui::ShowDemoWindow();
	}

	int shutdown()
	{
		bgfx::destroy(polyShader);
		delete sky;
		delete camera;
		return 0;
	}
private:
	entt::registry registry;
	bgfx::ProgramHandle polyShader;
	Model* barn;
	SkySystem* sky;
	Camera* camera;
};

IG_IMPLEMENT_MAIN(Sandbox);
