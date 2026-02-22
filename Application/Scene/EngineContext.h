#pragma once

#include <Base/OffScreen.h>
#include <3d/Camera/Camera.h>
#include <Base/Audio.h>
#include <Base/Input.h>

namespace MyEngine {

	struct EngineContext final {

		OffScreen* offScreen;
		Camera* camera;
		Audio* audio;
		Input* input;

	};
}