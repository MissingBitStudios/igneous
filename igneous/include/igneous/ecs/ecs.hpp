#pragma once

#include <entt/entt.hpp>

namespace igneous {
namespace ecs
{
	extern entt::registry registry;

	template <typename ...Components>
	entt::entity create(Components... components);

	template <typename ...Components>
	void assign(entt::entity entity, Components... components);
}
}

#include "igneous/ecs/ecs.inl"
