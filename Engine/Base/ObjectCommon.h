#pragma once
#include <2d/Object/Object2DCommon.h>
#include <3d/Object/Object3DCommon.h>
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