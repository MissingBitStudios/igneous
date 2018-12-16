#include "rendererSystem.h"

void render(bgfx::ProgramHandle program, entt::registry<> &registry) {
	registry.view<Model, Transformation>().each([program](const auto, auto &model, auto &transformation)
	{
		bgfx::setTransform(&transformation.mtx);
		bgfx::setVertexBuffer(0, model.mVbh);
		bgfx::setIndexBuffer(model.mIbh);
		bgfx::setState(BGFX_STATE_DEFAULT);
		bgfx::submit(0, program);
	});
}