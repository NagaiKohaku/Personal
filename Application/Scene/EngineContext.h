#pragma once

#include <Base/OffScreen.h>
#include <2d/Object/Object2DCommon.h>
#include <3d/Object/Object3DCommon.h>
#include <3d/Object/DebugObjectCommon.h>
#include <3d/Camera/Camera.h>
#include <Base/Audio.h>
#include <Base/Input.h>

namespace MyEngine {

	struct EngineContext final {

		OffScreen* offScreen;
		Object2DCommon* object2DCommon;
		Object3DCommon* object3DCommon;
		DebugObjectCommon* debugObjectCommon;
		Camera* camera;
		Audio* audio;
		Input* input;

	};
}