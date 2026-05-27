#pragma once

#include <Base/OffScreen.h>
#include <Object/Base/ObjectManager.h>
#include <Object/3D/Camera/CameraManager.h>
#include <Base/Audio.h>
#include <Base/Input.h>
#include <Base/Renderer.h>

struct EngineContext final {
	MyEngine::OffScreen* offScreen;
	MyEngine::ObjectManager* objectManager;
	MyEngine::CameraManager* cameraManager;
	MyEngine::Audio* audio;
	MyEngine::Input* input;
	MyEngine::Renderer* renderer;
};
