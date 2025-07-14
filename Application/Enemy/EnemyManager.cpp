#include "EnemyManager.h"

#include "3d/Collider/ColliderManager.h"

#include "LevelEditor/LevelDataLoader.h"

#include "numbers"

///=====================================================/// 
/// 初期化
///=====================================================///
void EnemyManager::Initialize(Camera* ptr) {

	//レベルデータローダーのインスタンスを取得
	levelDataLoader_ = LevelDataLoader::GetInstance();

	//カメラポインタを取得
	camera_ = ptr;

	//タイマーの設定
	spawnTimer_ = 0.0f;

	//スポーン間隔の設定
	spawnInterval_ = 1.0f;

	//最大数の設定
	spawnMaxSize_ = 4;

	//基準方向の設定
	spawnBaseAngle_ = std::numbers::pi_v<float> *0.25f;

	//距離の設定
	spawnDistance_ = 3.0f;

	//オフセットの設定
	spawnOffset_ = { 0.0f,4.0f,30.0f };

	levelDataLoader_->Load("EnemyFormation01.json");
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

		std::vector<LevelDataLoader::ObjectData> objectDatas = levelDataLoader_->PickObjectData("EnemyFormation01.json", LevelDataLoader::ENEMY);

		for (int i = 0; i < objectDatas.size(); i++) {

			//待機座標
			Vector3 standbyPos = objectDatas[i].position;

			//スポーン方向
			Vector3 spawnDirection = Normalize(Vector3(standbyPos.x,standbyPos.y,0.0f));

			//出現座標
			Vector3 entryPos = spawnDirection * (spawnDistance_ * 8.0f) + standbyPos;

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