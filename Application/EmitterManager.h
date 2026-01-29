#pragma once

#include <3d/Particle/EmitterGroup.h>
#include <3d/Camera/Camera.h>
#include <Math/WorldTransform.h>

#include <memory>
#include <vector>
#include <string>

class EmitterManager {

public:

	struct EmitterObject {
		std::string name;
		std::unique_ptr<EmitterGroup> object;
	};

public:

	static EmitterManager* GetInstance();

	void Initialize();

	void Update();

	void Draw();

	EmitterGroup* CreateEmitter(const std::string& emitterName, const std::string fileName, WorldTransform* parent = nullptr);

	void DeleteEmitter(EmitterGroup* emitterPtr);

private:

	Camera* camera_;

	std::vector<std::unique_ptr<EmitterObject>> emitters_;

	bool isUpdate_ = true;

public:

	void SetCamera(Camera* cameraPtr) { camera_ = cameraPtr; }

	void SetIsUpdate(bool flag) { isUpdate_ = flag; }

};