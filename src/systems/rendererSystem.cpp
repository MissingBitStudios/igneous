#include "rendererSystem.h"
#include "../components/cameraComponent.h"

void RendererSystem::render(bgfx::ProgramHandle program, entt::registry<> &registry) {
	registry.view<Model, Transformation>().each([program](const auto, auto &model, auto &transformation)
	{
		bgfx::setTransform(&transformation.mtx);
		bgfx::setVertexBuffer(0, model.mVbh);
		bgfx::setIndexBuffer(model.mIbh);
		bgfx::setState(BGFX_STATE_DEFAULT);
		bgfx::submit(0, program);
	});
}

void RendererSystem::useCamera(uint32_t entity, uint16_t width, uint16_t height, entt::registry<> &registry)
{
	Camera camera = registry.get<Camera>(entity);
	bgfx::setViewTransform(0, &camera.view[0][0], &camera.proj[0][0]);
	bgfx::setViewRect(0, 0, 0, width, height);
}