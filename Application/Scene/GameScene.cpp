#include "GameScene.h"

#include "Base/OffScreen.h"

#include "2d/Sprite/SpriteManager.h"
#include "3d/Model/ModelManager.h"
#include "2d/Sprite/TextureManager.h"

#include "3d/Object/Object3DCommon.h"
#include "3d/Object/DebugObjectCommon.h"
#include "3d/Object/SkyBoxCommon.h"

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
	camera_->SetDebugCameraFlag(true);

	//カメラの座標
	camera_->GetWorldTransform().translate_ = { 0.0f,3.0f,0.0f };

	/// === オブジェクトマネージャーの生成 === ///

	//バレットマネージャーの生成
	bulletManager_ = std::make_unique<BulletManager>();

	//バレットマネージャーの初期化
	bulletManager_->Initialize();

	//エネミーマネージャーの生成
	enemyManager_ = std::make_unique<EnemyManager>();

	//エネミーマネージャーの初期化
	enemyManager_->Initialize(camera_.get());

	/// === オブジェクトの生成 === ///

	//プレイヤーの生成
	player_ = std::make_unique<Player>();

	//プレイヤーの初期化
	player_->Initialize(camera_.get(), bulletManager_.get());

	//追尾カメラの生成
	followCamera_ = std::make_unique<FollowCamera>();

	//追尾カメラの初期化
	followCamera_->Initialize(camera_.get(), player_.get());

	/// === スカイボックスの生成 === ///

	//生成
	skyBox_ = std::make_unique<SkyBox>();

	//初期化
	skyBox_->Initialize("Resource/Texture/CubeTexture/Transparent.png");

	//スケールの設定
	skyBox_->GetWorldTransform().scale_ = { 10000.0f,10000.0f,10000.0f };

	//スカイボックスのテクスチャファイルパスを設定
	Object3DCommon::GetInstance()->SetTextureCubeFilePath(skyBox_->GetTextureFilePath());

	/// === 床のラインを生成 === ///

	const float lineDivide = 30.0f;

	const float lineDistance = 5.0f;

	for (size_t i = 0; i < lineDivide + 1; i++) {

		std::unique_ptr<DebugLine> newLine = std::make_unique<DebugLine>();

		newLine->Initialize(
			{ (i - lineDivide / 2.0f) * lineDistance, 0.0f, (-lineDivide / 2.0f) * lineDistance },
			{ (i - lineDivide / 2.0f) * lineDistance, 0.0f, (lineDivide / 2.0f) * lineDistance },
			{ 1.0f,1.0f,1.0f,1.0f }
		);

		if (i == static_cast<int>(lineDivide / 2.0f)) {
			newLine->SetColor({ 1.0f,0.0f,0.0f,1.0f });
		}

		lines_.push_back(std::move(newLine));
	}

	for (size_t i = 0; i < lineDivide + 1; i++) {

		std::unique_ptr<DebugLine> newLine = std::make_unique<DebugLine>();

		newLine->Initialize(
			{ (-lineDivide / 2.0f) * lineDistance, 0.0f, (i - lineDivide / 2.0f) * lineDistance },
			{ (lineDivide / 2.0f) * lineDistance, 0.0f, (i - lineDivide / 2.0f) * lineDistance },
			{ 1.0f,1.0f,1.0f,1.0f }
		);

		if (i == static_cast<int>(lineDivide / 2.0f)) {
			newLine->SetColor({ 0.0f,1.0f,0.0f,1.0f });
		}

		lines_.push_back(std::move(newLine));
	}
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

	//スカイボックスの更新
	skyBox_->Update();

	//プレイヤーの更新
	player_->Update();

	//エネミーの更新
	enemyManager_->Update();

	//弾の更新
	bulletManager_->Update();

	for (auto& line : lines_) {

		line->Update();
	}

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

	//スカイボックスの描画
	skyBox_->Draw();

	for (auto& line : lines_) {

		line->Draw(LayerType::Object);
	}
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