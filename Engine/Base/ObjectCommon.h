#pragma once
#include <Object/2D/Object2DCommon.h>
#include <Object/3D/Object3DCommon.h>
#include <3d/Particle/ParticleCommon.h>
#include <3d/Object/DebugObjectCommon.h>

namespace MyEngine {
	struct ObjectCommon final {
		Object2DCommon* object2DCommon;
		Object3DCommon* object3DCommon;
		ParticleCommon* particleCommon;
		DebugObjectCommon* debugObjectCommon;
	};
}