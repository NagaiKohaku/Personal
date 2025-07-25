#include "GameScene.h"

#include "Base/OffScreen.h"

#include "2d/Sprite/SpriteManager.h"
#include "3d/Model/ModelManager.h"
#include "2d/Sprite/TextureManager.h"

#include "3d/Object/Object3DCommon.h"
#include "3d/Object/DebugObjectCommon.h"
#include "3d/Object/SkyBoxCommon.h"

#include "LevelEditor/LevelDataLoader.h"

#include "imgui.h"

#include "numbers"

///=====================================================/// 
/// 初期化
///=====================================================///
void GameScene::Initialize() {

	/// === カメラの設定 === ///

	//カメラを生成
	camera_ = std::make_unique<Camera>();

	//カメラの初期化
	camera_->Initialize();

	//デバッグカメラを使用しない
	camera_->SetDebugCameraFlag(false);

	//カメラの座標
	camera_->GetWorldTransform().translate_ = { 0.0f,3.0f,0.0f };

	/// === オブジェクトマネージャーの生成 === ///

	//エネミーマネージャーの生成
	enemyManager_ = std::make_unique<EnemyManager>();

	//バレットマネージャーの生成
	bulletManager_ = std::make_unique<BulletManager>();

	//バレットマネージャーの初期化
	bulletManager_->Initialize();

	/// === オブジェクトの生成 === ///

	//プレイヤーの生成
	player_ = std::make_unique<Player>();

	//プレイヤーの初期化
	player_->Initialize(camera_.get(), bulletManager_.get());

	//エネミーマネージャーの初期化
	enemyManager_->Initialize(camera_.get(), bulletManager_.get(), player_.get());

	//追尾カメラの生成
	followCamera_ = std::make_unique<FollowCamera>();

	//追尾カメラの初期化
	followCamera_->Initialize(camera_.get(), player_.get());

	lineGround_ = std::make_unique<LineGround>();

	lineGround_->Initialize();

}

///=====================================================/// 
/// 終了処理
///=====================================================///
void GameScene::Finalize() {

	//音声データの解放
	Audio::GetInstance()->Finalize();
}

///=====================================================/// 
/// 更新
///=====================================================///
void GameScene::Update() {

	//追尾カメラの更新
	followCamera_->Update();

	//カメラをデバッグ状態で更新
	camera_->Update();

	//プレイヤーの更新
	player_->Update();

	//エネミーの更新
	enemyManager_->Update();

	//弾の更新
	bulletManager_->Update();

	lineGround_->Update();
}

///=====================================================/// 
/// 描画
///=====================================================///
void GameScene::Draw() {

	//プレイヤーの描画
	player_->Draw();

	//エネミーの描画
	enemyManager_->Draw();

	//弾の描画
	bulletManager_->Draw();

	lineGround_->Draw();
}

///=====================================================/// 
/// ImGui
///=====================================================///
void GameScene::ImGui() {

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