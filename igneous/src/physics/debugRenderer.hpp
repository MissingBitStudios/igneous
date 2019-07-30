#pragma once

#include <bgfx/bgfx.h>
#include <LinearMath/btIDebugDraw.h>

namespace igneous {
namespace physics
{
	class DebugRenderer : public btIDebugDraw
	{
	public:
		DebugRenderer();
		~DebugRenderer();

		void render();

		void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
		void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
		void reportErrorWarning(const char* warningString);
		void draw3dText(const btVector3& location, const char* textString);
		void setDebugMode(int debugMode);
		int getDebugMode() const;
	private:
		int debugMode;
	};
}
} // end namespace igneous
