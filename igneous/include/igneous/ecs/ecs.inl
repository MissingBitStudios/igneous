#pragma once

namespace igneous {
namespace ecs
{
	template <typename ...Components>
	entt::entity create(Components... components)
	{
		auto entity = registry.create();
		auto x = { (registry.assign<Components>(entity, components), 0)... };
		return entity;
	}

	template <typename ...Components>
	void assign(entt::entity entity, Components... components)
	{
		auto x = { (registry.assign<Components>(entity, components), 0)... };
	}
}
}
