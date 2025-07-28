#include "EnemyManager.h"

#include "3d/Collider/ColliderManager.h"

#include "Bullet/BulletManager.h"
#include "Player/Player.h"

#include "LevelEditor/LevelDataLoader.h"

#include "Math/Random.h"

#include "numbers"

///=====================================================/// 
/// 初期化
///=====================================================///
void EnemyManager::Initialize(Camera* cameraPtr, BulletManager* bulletPtr, Player* playerPtr) {

	//レベルデータローダーのインスタンスを取得
	levelDataLoader_ = LevelDataLoader::GetInstance();

	//カメラポインタを取得
	camera_ = cameraPtr;

	bulletManager_ = bulletPtr;

	player_ = playerPtr;

	directoryPath_ = "EnemyFormation/";

	//タイマーの設定
	spawnTimer_ = 0.0f;

	//スポーン間隔の設定
	spawnInterval_ = 3.0f;

	//最大数の設定
	spawnMaxSize_ = 4;

	//基準方向の設定
	spawnBaseAngle_ = std::numbers::pi_v<float> *0.25f;

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

	//タイマーを進ませる
	spawnTimer_ += 1.0f / 60.0f;

	//スポーン間隔を越えたら
	if (spawnTimer_ >= spawnInterval_) {

		int randomNum = RandomInt(1,levelDataLoader_->GetObjectDataCount(directoryPath_));

		Vector3 randomPos = RandomVector3(Vector3(-10.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 0.0f));

		std::string filePath = "EnemyFormation0" + std::to_string(randomNum) + ".json";

		std::vector<ObjectData> objectDatas = levelDataLoader_->PickObjectData(directoryPath_ + filePath, ObjectType::ENEMY);

		for (int i = 0; i < objectDatas.size(); i++) {

			//待機座標
			Vector3 standbyPos = objectDatas[i].position + randomPos;

			//スポーン方向
			Vector3 spawnDirection = Normalize(Vector3(standbyPos.x, standbyPos.y, 0.0f));

			//出現座標
			Vector3 entryPos = spawnDirection * (spawnDistance_ * 10.0f) + standbyPos;

			//エネミーをスポーンさせる
			Spawn(entryPos, standbyPos, objectDatas[i]);
		}

		//タイマーのリセット
		spawnTimer_ = 0.0f;
	}
}

///=====================================================/// 
/// スポーン処理
///=====================================================///
void EnemyManager::Spawn(Vector3 entryPos, Vector3 standbyPos, ObjectData objectData) {

	//エネミーを生成
	std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();

	//初期化
	newEnemy->Initialize(camera_, bulletManager_, player_, objectData);

	//初期座標を設定
	newEnemy->SetPosition(entryPos);

	//エントリー座標を設定
	newEnemy->SetEntryPos(entryPos);

	//待機座標を設定
	newEnemy->SetStandbyPos(standbyPos);

	//離脱開始座標を設定
	newEnemy->SetExitStartPos(Vector3(standbyPos.x, standbyPos.y, standbyPos.z - 5.0f));

	//離脱座標を設定
	newEnemy->SetExitPos(entryPos);

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