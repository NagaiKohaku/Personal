#include "ParticleEditorScene.h"

#include "2d/Sprite/SpriteManager.h"
#include "3d/Model/ModelManager.h"
#include "3d/Particle/ParticleManager.h"

#include "3d/Object/Object3DCommon.h"
#include "3d/Object/DebugObjectCommon.h"

#include "imgui.h"

void ParticleEditorScene::Initialize() {

	/// === カメラの設定 === ///

	//カメラを生成
	camera_ = std::make_unique<Camera>();

	camera_->SetDebugCameraFlag(true);

	//デフォルトカメラを設定
	Object3DCommon::GetInstance()->SetDefaultCamera(camera_.get());

	DebugObjectCommon::GetInstance()->SetDefaultCamera(camera_.get());

	ParticleManager::GetInstance()->SetDefaultCamera(camera_.get());

	std::unique_ptr<ParticleEmitter> newEmitter = std::make_unique<ParticleEmitter>();

	newEmitter->Initialize("default", camera_.get());

	emitters_.push_back(std::move(newEmitter));
}

void ParticleEditorScene::Update() {

	//カメラをデバッグ状態で更新
	camera_->Update();

	ImGui::Begin("ParticleEditor");

	for (auto& emitter : emitters_) {

		emitter->ImGui();

		emitter->Update();
	}

	ImGui::End();
}

void ParticleEditorScene::Draw() {

	for (auto& emitter : emitters_) {

		emitter->Draw(Object);
	}
}