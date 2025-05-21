#pragma once

#include "3d/Camera/Camera.h"
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

private:

	Camera* camera_;

	//エミッターのリスト
	std::list<std::unique_ptr<ParticleEmitter>> particleEmitters_;

	std::string name_;

	std::string directoryPath;
};