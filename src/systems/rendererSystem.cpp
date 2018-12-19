#include "rendererSystem.h"

#include <glm/gtc/matrix_transform.hpp>

#include "../components/modelComponent.h"
#include "../components/transformationComponent.h"

namespace RendererSystem
{
	void render(entt::registry<> &registry) {
		registry.view<ModelComponent, Transformation>().each([](const auto, auto &model, auto &transformation)
		{
			bgfx::setTransform(&transformation.mtx);
			bgfx::setVertexBuffer(0, model.mVbh);
			bgfx::setIndexBuffer(model.mIbh);
			bgfx::setTexture(0, model.uniform, model.texture);
			bgfx::setState(BGFX_STATE_DEFAULT);
			bgfx::submit(0, model.program);
		});
	}
}