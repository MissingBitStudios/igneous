#pragma once

#include <btBulletDynamicsCommon.h>

class PhysicsServer
{
public:
	static PhysicsServer& getInstance();

	void step(float dt);

	PhysicsServer(PhysicsServer const&) = delete;
	void operator=(PhysicsServer const&) = delete;
private:
	PhysicsServer();
	~PhysicsServer();

	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
};