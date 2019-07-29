#include "igneous/physics/physics.hpp"

#include <glm/gtc/type_ptr.hpp>

#include "igneous/core/log.hpp"
#include "igneous/ecs/ecs.hpp"
#include "igneous/ecs/components/transformationComponent.hpp"

namespace igneous {
namespace physics
{
	static btDefaultCollisionConfiguration* collisionConfiguration;
	static btCollisionDispatcher* dispatcher;
	static btBroadphaseInterface* overlappingPairCache;
	static btSequentialImpulseConstraintSolver* solver;
	static btDiscreteDynamicsWorld* dynamicsWorld;
	static btSphereShape sphere(10.0f);
	static btTransform identityTransform;

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

		identityTransform.setFromOpenGLMatrix(glm::value_ptr(glm::identity<glm::mat4>()));
		IG_CORE_INFO("Physics Initialized");
	}

	RigidBodyHandle loadRigidBody(std::string path)
	{
		btScalar mass = 10.0f;
		btCollisionShape* shape = &sphere;
		btTransform transform = identityTransform;

		assert((!shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE));

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			shape->calculateLocalInertia(mass, localInertia);

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects

#define USE_MOTIONSTATE 1
#ifdef USE_MOTIONSTATE
		btDefaultMotionState * myMotionState = new btDefaultMotionState(transform);

		btRigidBody::btRigidBodyConstructionInfo cInfo(mass, myMotionState, shape, localInertia);

		btRigidBody* body = new btRigidBody(cInfo);
		//body->setContactProcessingThreshold(m_defaultContactProcessingThreshold);

#else
		btRigidBody* body = new btRigidBody(mass, 0, shape, localInertia);
		body->setWorldTransform(startTransform);
#endif  //

		body->setUserIndex(-1);
		dynamicsWorld->addRigidBody(body);
		return body;
	}

	void update(float dt)
	{
		dynamicsWorld->stepSimulation(dt);
		ecs::registry.view<RigidBodyHandle, Transformation>().each([&](const auto, auto& rigidBody, auto& transformation)
		{
				btTransform t = rigidBody->getWorldTransform();
				t.getOpenGLMatrix(glm::value_ptr(transformation));
		});
	}

	void renderDebug(int debugFlags)
	{
		if (dynamicsWorld && dynamicsWorld->getDebugDrawer())
		{
			dynamicsWorld->getDebugDrawer()->setDebugMode(debugFlags);
			dynamicsWorld->debugDrawWorld();
		}
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
