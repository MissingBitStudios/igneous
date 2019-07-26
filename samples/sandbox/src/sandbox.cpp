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
		gui::setTheme(gui::Theme::CHERRY);

		sky = new SkySystem;

		polyShader = renderer::loadProgram("vs_poly", "vs_poly");

		barn = renderer::loadModel<GenericVertex>("res/models/BigBarn/BigBarn.obj", polyShader);

		auto entity = registry.create();
		registry.assign<ModelHandle>(entity, barn);
		registry.assign<Transformation>(entity, glm::identity<glm::mat4>());

		camera = new FPSCamera(glm::vec3(0.0f, 5.0f, 15.0f));

		input::setCursorVisible(false);
	}

	void update(float dt)
	{
		camera->update(dt);
		sky->update(dt);
	}

	void render()
	{
		camera->use(input::width, input::height);
		RendererSystem::render(registry);

		ImGui::ShowDemoWindow();
	}

	int shutdown()
	{
		delete sky;
		delete camera;
		return 0;
	}
private:
	entt::registry registry;
	bgfx::ProgramHandle polyShader;
	ModelHandle barn;
	SkySystem* sky;
	Camera* camera;
};

IG_IMPLEMENT_MAIN(Sandbox);
