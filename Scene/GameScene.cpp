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

	/// === オブジェクトの生成 === ///

	bulletManager_ = std::make_unique<BulletManager>();

	bulletManager_->Initialize();

	//プレイヤーの生成
	player_ = std::make_unique<Player>();

	//カメラをセット
	player_->SetCamera(camera_.get());

	//プレイヤーの初期化
	player_->Initialize();

	//バレットマネージャーをセット
	player_->SetBulletManager(bulletManager_.get());

	spawnTime_ = 3.0f;

	spawnTimer_ = 0.0f;

	spawnMaxSize_ = 4;

	spawnPos_[0] = { 4.0f,6.0f,30.0f };

	spawnPos_[1] = { -4.0f,6.0f,30.0f };

	spawnPos_[2] = { 4.0f,-2.0f,30.0f };

	spawnPos_[3] = { -4.0f,-2.0f,30.0f };
}

void GameScene::Finalize() {

	//音声データの解放
	Audio::GetInstance()->Finalize();
}

void GameScene::Update() {

	EnemySpawn();

	enemies_.remove_if([](const std::unique_ptr<Enemy>& enemy) {
		if (enemy->GetCanRemove()) {
			return true;
		}
		return false;
		});

	//カメラをデバッグ状態で更新
	camera_->Update();

	//プレイヤーの更新
	player_->Update();

	for (auto& enemy : enemies_) {

		enemy->Update();
	}

	//弾の更新
	bulletManager_->Update();

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
	player_->Draw();

	for (auto& enemy : enemies_) {

		enemy->Draw();
	}

	//弾の描画
	bulletManager_->Draw();
}

void GameScene::ImGui() {
}

void GameScene::EnemySpawn() {

	if (enemies_.size() >= spawnMaxSize_) {
		return;
	}

	spawnTimer_ += 1.0f / 60.0f;

	if (spawnTimer_ >= spawnTime_) {

		std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();

		newEnemy->SetCamera(camera_.get());

		newEnemy->Initialize();

		newEnemy->SetPosition(spawnPos_[enemies_.size()]);

		enemies_.push_back(std::move(newEnemy));

		spawnTimer_ = 0.0f;
	}
}