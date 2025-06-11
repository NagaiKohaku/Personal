#include "GameScene.h"

#include "2d/Sprite/SpriteManager.h"
#include "3d/Model/ModelManager.h"
#include "3d/Particle/ParticleManager.h"
#include "LevelDataLoader.h"

#include "3d/Object/Object3DCommon.h"
#include "3d/Object/DebugObjectCommon.h"

#include "imgui.h"
#include "json.hpp"
#include "fstream"

#include "numbers"

void GameScene::Initialize() {

	/// === カメラの設定 === ///

	//カメラを生成
	camera_ = std::make_unique<Camera>();

	camera_->SetDebugCameraFlag(true);

	//カメラの座標
	camera_->GetWorldTransform().translate_ = { 0.0f,3.0f,0.0f };

	//デフォルトカメラを設定
	Object3DCommon::GetInstance()->SetDefaultCamera(camera_.get());

	DebugObjectCommon::GetInstance()->SetDefaultCamera(camera_.get());

	ParticleManager::GetInstance()->SetDefaultCamera(camera_.get());

	/// === リソースの読み込み === ///

	/// === オブジェクトの生成 === ///

	LevelDataLoader::GetInstance()->Load("LevelData.json");

	for (auto& object : LevelDataLoader::GetInstance()->GetObjects()) {

		//オブジェクトの生成
		std::unique_ptr<Object3D> newObject = std::make_unique<Object3D>();

		//モデルを設定
		newObject->SetModel(object.filename);

		//トランスフォームのパラメータ設定
		newObject->GetWorldTransform().translate_ = object.position;
		newObject->GetWorldTransform().rotate_ = object.rotation;
		newObject->GetWorldTransform().scale_ = object.scale;

		//オブジェクトをコンテナに追加
		objects_.push_back(std::move(newObject));
	}

	//bulletManager_ = std::make_unique<BulletManager>();

	//bulletManager_->Initialize();

	////プレイヤーの生成
	//player_ = std::make_unique<Player>();

	//player_->SetCamera(camera_.get());

	////プレイヤーの初期化
	//player_->Initialize();

	//player_->SetBulletManager(bulletManager_.get());

}

void GameScene::Finalize() {

	//音声データの解放
	Audio::GetInstance()->Finalize();
}

void GameScene::Update() {

	//カメラをデバッグ状態で更新
	camera_->Update();

	//プレイヤーの更新
	//player_->Update();

	//弾の更新
	//bulletManager_->Update();

	//オブジェクトの更新
	for (auto& object : objects_) {
		//オブジェクトの更新
		object->Update();
	}

	//ImGuiを起動
	ImGui::Begin("Scene");

	if (ImGui::TreeNode("Camera")) {

		camera_->DisplayImGui();

		ImGui::TreePop();
	}

	ImGui::Text("Shift + LeftClick : Move Camera");
	ImGui::Text("Shift + RightClick : Rotate Camera");
	ImGui::Text("Shift + MiddleWheel : Move Offset Camera");

	//ImGuiの終了
	ImGui::End();

}

void GameScene::Draw() {

	//プレイヤーの描画
	//player_->Draw();

	//弾の描画
	//bulletManager_->Draw();

	for (auto& object : objects_) {
		//オブジェクトの描画
		object->Draw(LayerType::Object);
	}
}

void GameScene::ImGui() {
}