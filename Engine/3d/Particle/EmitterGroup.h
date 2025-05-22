#pragma once

#include "3d/Camera/Camera.h"
#include "Math/WorldTransform.h"
#include "ParticleEmitter.h"

#include "list"

class EmitterGroup {

public:

	void Initialize(Camera* ptr);

	void Update();

	void Draw();

	void ImGui();

	void LoadEmitter(std::string fileName);

	void SaveEmitter();

	void AddEmitter();

	void Emit();

public:

	std::list<ParticleEmitter*> GetEmitterList();

	std::string GetName() { return name_; }

	void SetTextureList(std::vector<std::string> list) { textureList_ = list; }

	void SetWorldTransform(WorldTransform worldTransform) { transform_ = worldTransform; }

private:

	Camera* camera_;

	//エミッターのリスト
	std::list<std::unique_ptr<ParticleEmitter>> particleEmitters_;

	std::vector<std::string> textureList_;

	WorldTransform transform_;

	std::string name_;

	std::string directoryPath;
};