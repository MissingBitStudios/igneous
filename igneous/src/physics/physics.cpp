#include "igneous/physics/physics.hpp"

#include "igneous/core/log.hpp"

namespace igneous {
namespace physics
{
	static btDefaultCollisionConfiguration* collisionConfiguration;
	static btCollisionDispatcher* dispatcher;
	static btBroadphaseInterface* overlappingPairCache;
	static btSequentialImpulseConstraintSolver* solver;
	static btDiscreteDynamicsWorld* dynamicsWorld;

	void init()
	{
		IG_CORE_INFO("Initializing Physics");
		///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
		collisionConfiguration = new btDefaultCollisionConfiguration();

		///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
		dispatcher = new btCollisionDispatcher(collisionConfiguration);

		///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
		overlappingPairCache = new btDbvtBroadphase();

		///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
		solver = new btSequentialImpulseConstraintSolver;

		dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

		dynamicsWorld->setGravity(btVector3(0, -10, 0));
		dynamicsWorld->debugDrawWorld();
		IG_CORE_INFO("Physics Initialized");
	}

	void update(float dt)
	{
		dynamicsWorld->stepSimulation(dt, 10);
	}

	void shutdown()
	{
		delete dynamicsWorld;
		delete solver;
		delete overlappingPairCache;
		delete dispatcher;
		delete collisionConfiguration;
	}
}
} // end namespace igneous
