#include "EnemyManager.h"

#include "Collider/ColliderManager.h"

#include "numbers"

///=====================================================/// 
/// 初期化
///=====================================================///
void EnemyManager::Initialize(Camera* ptr) {

	//カメラポインタを取得
	camera_ = ptr;

	//タイマーの設定
	spawnTimer_ = 0.0f;

	//スポーン間隔の設定
	spawnInterval_ = 1.0f;

	//最大数の設定
	spawnMaxSize_ = 4;

	//基準方向の設定
	spawnBaseAngle_ = std::numbers::pi_v<float> * 0.25f;

	//距離の設定
	spawnDistance_ = 3.0f;

	//オフセットの設定
	spawnOffset_ = { 0.0f,4.0f,30.0f };
}

///=====================================================/// 
/// 更新
///=====================================================///
void EnemyManager::Update() {

	//スポーン更新
	SpawnUpdate();

	//削除処理
	DeleteEnemy();

	for (auto& enemy : enemies_) {

		//エネミーの更新
		enemy->Update();
	}
}

///=====================================================/// 
/// 描画
///=====================================================///
void EnemyManager::Draw() {

	for (auto& enemy : enemies_) {

		//エネミーの描画
		enemy->Draw();
	}
}

///=====================================================/// 
/// スポーン更新
///=====================================================///
void EnemyManager::SpawnUpdate() {

	//エネミーが残っていれば早期リターン
	if (enemies_.size() != 0) {
		return;
	}

	//タイマーを進ませる
	spawnTimer_ += 1.0f / 60.0f;

	//スポーン間隔を越えたら
	if (spawnTimer_ >= spawnInterval_) {

		for (int i = 0; i < spawnMaxSize_; i++) {

			//スポーン角度
			float spawnAngle = spawnBaseAngle_ + i * ((std::numbers::pi_v<float> * 2.0f) / spawnMaxSize_);

			//スポーン方向
			Vector3 spawnDirection = { std::cosf(spawnAngle),std::sinf(spawnAngle),0.0f };

			//待機座標
			Vector3 standbyPos = spawnDirection * spawnDistance_ + spawnOffset_;

			//出現座標
			Vector3 entryPos = spawnDirection * (spawnDistance_ * 8.0f) + spawnOffset_;

			//エネミーをスポーンさせる
			Spawn(entryPos, standbyPos);
		}

		//タイマーのリセット
		spawnTimer_ = 0.0f;
	}
}

///=====================================================/// 
/// スポーン処理
///=====================================================///
void EnemyManager::Spawn(Vector3 entryPos, Vector3 standbyPos) {

	//エネミーを生成
	std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();

	//初期化
	newEnemy->Initialize(camera_);

	//初期座標を設定
	newEnemy->SetPosition(entryPos);

	//待機座標を設定
	newEnemy->SetStandbyPos(standbyPos);

	//リストに登録
	enemies_.push_back(std::move(newEnemy));
}

///=====================================================/// 
/// 削除処理
///=====================================================///
void EnemyManager::DeleteEnemy() {

	//エネミーの削除
	enemies_.remove_if([](const std::unique_ptr<Enemy>& enemy) {
		if (enemy->GetCanRemove()) {
			return true;
		}
		return false;
		});
}