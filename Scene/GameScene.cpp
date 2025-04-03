#include "GameScene.h"

#include "2d/Sprite/SpriteManager.h"
#include "3d/Model/ModelManager.h"
#include "3d/Particle/ParticleManager.h"

#include "3d/Object/Object3DCommon.h"
#include "3d/Object/DebugObjectCommon.h"

#include "imgui.h"

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

	//モデルのロード
	ModelManager::GetInstance()->LoadModel("Ground", "terrain");

	//音声データの読み込み
	Audio::GetInstance()->SoundLoad("SE", "se.wav");

	/// === オブジェクトの生成 === ///

	/// === 箱の生成 === ///

	//箱の生成
	cube_ = std::make_unique<Object3D>();

	//座標の設定
	cube_->GetWorldTransform().translate_ = { 0.0f,1.0f,0.0f };

	//モデルの設定
	cube_->SetModel("Cube");

	//モデルの色を指定
	cube_->GetModel()->SetColor({ 0.5f,0.0f,0.0f,1.0f });

	/// === 球の生成 === ///

	//球の生成
	ball_ = std::make_unique<Object3D>();

	//座標の設定
	ball_->GetWorldTransform().translate_ = { 0.0f,3.0f,0.0f };

	//モデルの設定
	ball_->SetModel("Sphere");

	//モデルの色の指定
	ball_->GetModel()->SetColor({ 0.5f,0.f,0.0f,1.0f });

	camera_->SetTrackingObject(ball_.get());

	/// === 地面の生成 === ///

	//地面の生成
	ground_ = std::make_unique<Object3D>();

	//角度の設定
	ground_->GetWorldTransform().rotate_ = { 0.0f,static_cast<float>(std::numbers::pi) / 180.0f * -90.0f,0.0f };

	//モデルの設定
	ground_->SetModel("Ground");
}

void GameScene::Finalize() {

	//音声データの解放
	Audio::GetInstance()->Finalize();
}

void GameScene::Update() {

	//カメラをデバッグ状態で更新
	camera_->Update();

	//3Dオブジェクトの更新
	cube_->Update();

	ball_->Update();

	ground_->Update();

	//ImGuiを起動
	ImGui::Begin("Scene");

	//モデルのImGui
	if (ImGui::TreeNode("Cube")) {

		cube_->DisplayImGui();

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Ball")) {

		ball_->DisplayImGui();

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Camera")) {

		camera_->DisplayImGui();

		ImGui::TreePop();
	}

	if (ImGui::Button("Start Audio")) {

		//音声データの再生
		Audio::GetInstance()->StartSound("SE",false);
	}

	ImGui::Text("Shift + LeftClick : Move Camera");
	ImGui::Text("Shift + RightClick : Rotate Camera");
	ImGui::Text("Shift + MiddleWheel : Move Offset Camera");

	//ImGuiの終了
	ImGui::End();

}

void GameScene::Draw() {

	//箱の描画 : オブジェクトレイヤー
	cube_->Draw(Object);

	//球の描画 : オブジェクトレイヤー
	ball_->Draw(Object);

	//地面の描画 : オブジェクトレイヤー
	ground_->Draw(Object);
}