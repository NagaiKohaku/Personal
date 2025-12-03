#include "EnemyManager.h"

#include "3d/Collider/ColliderManager.h"

#include "Bullet/BulletManager.h"
#include "Player/Player.h"

#include "LevelEditor/LevelDataLoader.h"
#include "ObjectManager.h"

#include "Math/Random.h"

#include "numbers"

///=====================================================/// 
/// 敵の生成管理に必要な各種パラメータや参照ポインタを初期化
///=====================================================///
void EnemyManager::Initialize(Camera* cameraPtr, BulletManager* bulletPtr, Player* playerPtr) {

	//レベルデータローダーのインスタンスを取得
	levelDataLoader_ = LevelDataLoader::GetInstance();

	//カメラポインタを取得
	camera_ = cameraPtr;

	//バレットマネージャーポインタを取得
	bulletManager_ = bulletPtr;

	//プレイヤーポインタを取得
	player_ = playerPtr;

	//ディレクトリパスの設定
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

	//範囲の設定
	spawnRange_ = { 10.0f,7.0f,0.0f };

	//スポーンフラグの設定
	isSpawn_ = true;
}

///=====================================================/// 
/// 敵の生成処理および各敵オブジェクトの更新処理
///=====================================================///
void EnemyManager::Update() {

	//スポーン許可なら
	if (isSpawn_) {

		//スポーン更新
		SpawnUpdate();

		//削除処理
		DeleteEnemy();
	}

	for (auto& enemy : enemies_) {

		//エネミーの更新
		enemy->Update();
	}
}

///=====================================================/// 
/// 管理中の全ての敵オブジェクトに対して、座標変換のみの更新処理
///=====================================================///
void EnemyManager::TransformUpdate() {

	for (auto& enemy : enemies_) {

		//エネミーの座標のみ更新
		enemy->TransformUpdate();
	}
}

///=====================================================/// 
/// 管理中の全ての敵オブジェクトを描画
///=====================================================///
void EnemyManager::Draw() {

	for (auto& enemy : enemies_) {

		//エネミーの描画
		enemy->Draw();
	}
}

///=====================================================/// 
/// 全敵に対してクリア演出を適用
///=====================================================///
void EnemyManager::StartClearUpdate() {

	//スポーン停止
	isSpawn_ = false;

	for (auto& enemy : enemies_) {

		//クリア用爆発エミッターを起動
		enemy->EmitClearExplosive();

		for (int i = 0; i < 4; i++) {

			//クリア用更新
			enemy->ClearUpdate();
		}
	}
}

///==========================================================/// 
/// 敵のスポーン処理を更新し、一定間隔で編成データに基づく複数の敵を出現
///==========================================================///
void EnemyManager::SpawnUpdate() {

	//タイマーを進ませる
	spawnTimer_ += 1.0f / 60.0f;

	//スポーン間隔を越えたら
	if (spawnTimer_ >= spawnInterval_) {

		//編成データの番号をランダムに決定
		int randomNum = RandomInt(1,levelDataLoader_->GetObjectDataCount(directoryPath_));

		//ランダムな出現座標を決定
		Vector3 randomPos = RandomVector3(Vector3(-spawnRange_.x, 0.0f, -spawnRange_.z), Vector3(spawnRange_.x, spawnRange_.y, spawnRange_.z));

		//編成データのファイルパスを決定
		std::string filePath = "EnemyFormation0" + std::to_string(randomNum) + ".json";

		//編成データを取得
		std::vector<ObjectData> objectDatas = levelDataLoader_->PickObjectData(directoryPath_ + filePath, ObjectType::ENEMY);

		//編成データを基にエネミーをスポーンさせる
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

///============================================================/// 
/// 敵を生成し、初期座標・エントリー座標・待機座標などの各種パラメータを設定
///============================================================///
void EnemyManager::Spawn(Vector3 entryPos, Vector3 standbyPos, ObjectData objectData) {

	//エネミーを生成
	ObjectManager::GetInstance()->SpawnEnemy();

	//エネミーリストに追加
	enemies_.push_back(ObjectManager::GetInstance()->GetEnemies().back());

	//初期化
	enemies_.back()->Initialize(camera_, bulletManager_, player_, objectData);

	//初期座標を設定
	enemies_.back()->SetPosition(entryPos);

	//エントリー座標を設定
	enemies_.back()->SetEntryPos(entryPos);

	//待機座標を設定
	enemies_.back()->SetStandbyPos(standbyPos);

	//離脱開始座標を設定
	enemies_.back()->SetExitStartPos(Vector3(standbyPos.x, standbyPos.y, standbyPos.z - 5.0f));

	//離脱座標を設定
	enemies_.back()->SetExitPos(entryPos);

}

///=====================================================/// 
/// 管理している敵オブジェクトの削除処理
///=====================================================///
void EnemyManager::DeleteEnemy() {

	//エネミーの削除
	enemies_.remove_if([](Enemy* enemy) {
		if (enemy->GetCanRemove()) {
			enemy->SetIsRemove(true);
			return true;
		}
		return false;
		});
}

///=====================================================/// 
/// エネミーリストのゲッター
///=====================================================///
std::list<Enemy*> EnemyManager::GetEnemyList() {

	std::list<Enemy*> enemyList;

	for (auto& enemy : enemies_) {
		enemyList.push_back(enemy);
	}

	return enemyList;
}