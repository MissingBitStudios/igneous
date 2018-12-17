#include "rendererSystem.h"

#include <glm/gtc/matrix_transform.hpp>

#include "../components/cameraComponent.h"
#include "../components/modelComponent.h"
#include "../components/transformationComponent.h"

namespace RendererSystem
{
	void render(entt::registry<> &registry) {
		registry.view<Model, Transformation>().each([](const auto, auto &model, auto &transformation)
		{
			bgfx::setTransform(&transformation.mtx);
			bgfx::setVertexBuffer(0, model.mVbh);
			bgfx::setIndexBuffer(model.mIbh);
			bgfx::setTexture(0, model.uniform, model.texture);
			bgfx::setState(BGFX_STATE_DEFAULT);
			bgfx::submit(0, model.program);
		});
	}

	void useCamera(uint32_t entity, uint16_t width, uint16_t height, entt::registry<> &registry)
	{
		Camera camera = registry.get<Camera>(entity);
		Transformation transform = registry.get<Transformation>(entity);
		glm::vec3 pos = transform.mtx[3];
		glm::mat4 view = glm::lookAt(pos, camera.focus, glm::vec3(0.0f, 1.0f, 0.0f));
		bgfx::setViewTransform(0, &view, &camera.proj[0][0]);
		bgfx::setViewRect(0, 0, 0, width, height);
	}
}