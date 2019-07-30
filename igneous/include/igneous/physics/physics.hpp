#pragma once

#include <string>

#include <btBulletDynamicsCommon.h>

namespace igneous {

typedef btRigidBody* RigidBodyHandle;

enum	DebugDrawModes
{
	DBG_NoDebug = 0,
	DBG_DrawWireframe = 1,
	DBG_DrawAabb = 2,
	DBG_DrawFeaturesText = 4,
	DBG_DrawContactPoints = 8,
	DBG_NoDeactivation = 16,
	DBG_NoHelpText = 32,
	DBG_DrawText = 64,
	DBG_ProfileTimings = 128,
	DBG_EnableSatComparison = 256,
	DBG_DisableBulletLCP = 512,
	DBG_EnableCCD = 1024,
	DBG_DrawConstraints = (1 << 11),
	DBG_DrawConstraintLimits = (1 << 12),
	DBG_FastWireframe = (1 << 13),
	DBG_DrawNormals = (1 << 14),
	DBG_DrawFrames = (1 << 15),
	DBG_MAX_DEBUG_DRAW_MODE
};

namespace physics
{
	void init();
	RigidBodyHandle loadRigidBody(std::string path);
	void update(float dt);
	void renderDebug(int debugFlags);
	void shutdown();
}
} // end namespace igneous
