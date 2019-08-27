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

		ModelHandle barn = renderer::loadModel("res/models/barn.bin");
		RigidBodyHandle rigidBody = physics::loadRigidBody("res/models/barn.bin");

		auto entity = ecs::create<ModelHandle, Transformation, RigidBodyHandle>(barn, glm::identity<glm::mat4>(), rigidBody);

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
		renderer::render();

		ImGui::ShowDemoWindow();
	}

	int shutdown()
	{
		delete sky;
		delete camera;
		return 0;
	}
private:
	SkySystem* sky;
	Camera* camera;
};

IG_IMPLEMENT_MAIN(Sandbox);
