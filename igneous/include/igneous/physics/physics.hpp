#pragma once

#include <string>

#include <btBulletDynamicsCommon.h>

namespace igneous {

typedef btRigidBody* RigidBodyHandle;

namespace physics
{
	void init();
	RigidBodyHandle loadRigidBody(std::string path);
	void update(float dt);
	void renderDebug(int debugFlags);
	void shutdown();
}
} // end namespace igneous
