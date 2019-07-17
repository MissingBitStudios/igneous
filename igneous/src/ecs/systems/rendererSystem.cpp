#include "ecs/systems/rendererSystem.hpp"

#include <bimg/bimg.h>
#include <glm/gtc/matrix_transform.hpp>

#include "ecs/components/modelComponent.hpp"
#include "ecs/components/transformationComponent.hpp"
#include "renderer/renderer.hpp"

namespace igneous {
namespace RendererSystem
{
	bgfx::TextureHandle checkerBoard;

	void init()
	{
		const uint32_t checkerBoardSize = 64;
		{
			const bgfx::Memory* mem = bgfx::alloc(checkerBoardSize * checkerBoardSize * 4);
			bimg::imageCheckerboard(mem->data, checkerBoardSize, checkerBoardSize, 8, 0xffe75480, 0xff000000);
			checkerBoard = bgfx::createTexture2D(checkerBoardSize, checkerBoardSize, false, 1
				, bgfx::TextureFormat::BGRA8
				, 0
				| BGFX_SAMPLER_MIN_POINT
				| BGFX_SAMPLER_MIP_POINT
				| BGFX_SAMPLER_MAG_POINT
				, mem
			);
		}
	}

	void render(entt::registry &registry)
	{
		bgfx::UniformHandle s_tex = bgfx::createUniform("s_tex", bgfx::UniformType::Sampler);
		registry.view<ModelComponent, Transformation>().each([s_tex](const auto, auto &model, auto &transformation)
		{
			for (Mesh* mesh : model.model->meshes)
			{
				bgfx::setTransform(&transformation.mtx);
				bgfx::setVertexBuffer(0, mesh->vbh);
				bgfx::setIndexBuffer(mesh->ibh);
				if (mesh->textures.size() > 0)
					bgfx::setTexture(0, s_tex, mesh->textures[0]);
				else
					bgfx::setTexture(0, s_tex, checkerBoard);
				bgfx::setState(BGFX_STATE_DEFAULT | BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA));
				bgfx::submit(0, model.program);
			}
		});
		bgfx::destroy(s_tex);
	}

	void shutdown()
	{
		bgfx::destroy(checkerBoard);
	}
}
} // end namespace igneous
