#pragma once

#include <btBulletDynamicsCommon.h>

class Physics
{
public:
	static Physics& getInstance();

	void step(float dt);

	Physics(Physics const&) = delete;
	void operator=(Physics const&) = delete;
private:
	Physics();
	~Physics();

	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
};