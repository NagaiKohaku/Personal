#pragma once

#include <Base/OffScreen.h>
#include <Object/Manager/ObjectManager.h>
#include <Base/ObjectCommon.h>
#include <3d/Camera/Camera.h>
#include <Base/Audio.h>
#include <Base/Input.h>
#include <Base/Renderer.h>

struct EngineContext final {
	MyEngine::OffScreen* offScreen;
	MyEngine::ObjectCommon objectCommon;
	ObjectManager* objectManager;
	MyEngine::Camera* camera;
	MyEngine::Audio* audio;
	MyEngine::Input* input;
	MyEngine::Renderer* renderer;
};
