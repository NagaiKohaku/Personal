#pragma once

#include <3d/Particle/EmitterGroup.h>
#include <3d/Particle/ParticleCommon.h>
#include <3d/Camera/Camera.h>
#include <Base/Input.h>
#include <Base/Renderer.h>
#include <Math/Transform/WorldTransform.h>

#include <memory>
#include <vector>
#include <string>

class EmitterManager {

public:

	struct EmitterObject {
		std::string name;
		std::unique_ptr<MyEngine::EmitterGroup> object;
	};

public:

	static EmitterManager* GetInstance();

	void Initialize(MyEngine::ParticleCommon* particleCommonPtr, MyEngine::Input* inputPtr, MyEngine::Renderer* rendererPtr);

	void Update();

	void Draw();

	MyEngine::EmitterGroup* CreateEmitter(const std::string& emitterName, const std::string fileName, MyEngine::WorldTransform* parent = nullptr);

	void DeleteEmitter(MyEngine::EmitterGroup* emitterPtr);

private:

	MyEngine::ParticleCommon* particleCommon_;

	MyEngine::Camera* camera_;

	MyEngine::Input* input_;

	MyEngine::Renderer* renderer_;

	std::vector<std::unique_ptr<EmitterObject>> emitters_;

	bool isUpdate_ = true;

public:

	void SetCamera(MyEngine::Camera* cameraPtr) { camera_ = cameraPtr; }

	void SetIsUpdate(bool flag) { isUpdate_ = flag; }

};