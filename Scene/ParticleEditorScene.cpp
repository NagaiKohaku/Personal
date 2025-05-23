#include "ParticleEditorScene.h"

#include "2d/Sprite/SpriteManager.h"
#include "2d/Sprite/TextureManager.h"
#include "3d/Model/ModelManager.h"
#include "3d/Particle/ParticleManager.h"

#include "3d/Object/Object3DCommon.h"
#include "3d/Object/DebugObjectCommon.h"

#include "imgui.h"
#include "fstream"
#include "filesystem"

void ParticleEditorScene::Initialize() {

	/// === カメラの設定 === ///

	//カメラを生成
	camera_ = std::make_unique<Camera>();

	camera_->SetDebugCameraFlag(true);

	//デフォルトカメラを設定
	Object3DCommon::GetInstance()->SetDefaultCamera(camera_.get());

	DebugObjectCommon::GetInstance()->SetDefaultCamera(camera_.get());

	ParticleManager::GetInstance()->SetDefaultCamera(camera_.get());

	for (const auto& entry : std::filesystem::directory_iterator("Resource/Sprite/Particle/")) {
		if (entry.path().extension() == ".png") {
			textureList_.push_back(entry.path().filename().string());
		}
	}

	for (auto& textureName : textureList_) {

		TextureManager::GetInstance()->LoadTexture("Resource/Sprite/Particle/" + textureName);
	}

	CreateGroup();

	for (size_t i = 0; i < 12; i++) {

		std::unique_ptr<DebugLine> newLine = std::make_unique<DebugLine>();

	}

}

void ParticleEditorScene::Update() {

	//カメラをデバッグ状態で更新
	camera_->Update();

	for (auto& group : emitterGroups_) {

		group->Update();
	}

}

void ParticleEditorScene::Draw() {

	for (auto& group : emitterGroups_) {

		group->Draw();
	}
}

void ParticleEditorScene::ImGui() {

	ImGui::Begin("ParticleEditor", nullptr, ImGuiWindowFlags_MenuBar);

	if (ImGui::BeginMenuBar()) {

		if (ImGui::BeginMenu("メニュー", "MENU")) {

			if (ImGui::MenuItem("新しいグループを生成")) {

				CreateGroup();
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	for (auto& group : emitterGroups_) {
		group->ImGui();
	}

	ImGui::End();
}

void ParticleEditorScene::CreateGroup() {

	std::unique_ptr<EmitterGroup> newGroup = std::make_unique<EmitterGroup>();

	newGroup->Initialize(camera_.get());

	newGroup->SetTextureList(textureList_);

	newGroup->LoadEmitter("defaultGroup");

	emitterGroups_.push_back(std::move(newGroup));
}