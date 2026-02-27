#pragma once

#include <Base/OffScreen.h>
#include <Base/ObjectCommon.h>
#include <3d/Camera/Camera.h>
#include <Base/Audio.h>
#include <Base/Input.h>
#include <Base/Renderer.h>

struct EngineContext final {
	MyEngine::OffScreen* offScreen;
	MyEngine::ObjectCommon objectCommon;
	MyEngine::Camera* camera;
	MyEngine::Audio* audio;
	MyEngine::Input* input;
	MyEngine::Renderer* renderer;
};
