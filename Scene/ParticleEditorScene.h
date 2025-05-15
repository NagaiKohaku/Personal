#pragma once
#include "BaseScene.h"

#include "Base/Audio.h"
#include "2d/Object/Object2D.h"
#include "3d/Camera/Camera.h"
#include "3d/Object/Object3D.h"
#include "3d/Object/DebugLine.h"
#include "3d/Particle/ParticleEmitter.h"

#include "memory"

class ParticleEditorScene : public BaseScene {

public:

	void Initialize() override;

	void Update() override;

	void Draw() override;

private:

	//カメラ
	std::unique_ptr<Camera> camera_;

	std::list<std::unique_ptr<ParticleEmitter>> emitters_;

};