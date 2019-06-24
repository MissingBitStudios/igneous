#include "ecs/systems/rendererSystem.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "ecs/components/modelComponent.hpp"
#include "ecs/components/transformationComponent.hpp"
#include "renderer/renderer.hpp"

namespace RendererSystem
{
	void render(entt::registry &registry, bgfx::TextureHandle handle)
	{
		bgfx::UniformHandle s_tex = bgfx::createUniform("s_tex", bgfx::UniformType::Sampler);
		registry.view<ModelComponent, Transformation>().each([s_tex, handle](const auto, auto &model, auto &transformation)
		{
			for (Mesh* mesh : model.model->meshes)
			{
				bgfx::setTransform(&transformation.mtx);
				bgfx::setVertexBuffer(0, mesh->vbh);
				bgfx::setIndexBuffer(mesh->ibh);
				if (mesh->textures.size() > 0)
					bgfx::setTexture(0, s_tex, mesh->textures[0]);
				else
					bgfx::setTexture(0, s_tex, handle);
				bgfx::setState(BGFX_STATE_DEFAULT | BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA));
				bgfx::submit(0, model.program);
			}
		});
		bgfx::destroy(s_tex);
	}
}