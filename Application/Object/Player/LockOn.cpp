#include "LockOn.h"

#include "3d/Camera/Camera.h"
#include <Object/Player/Player.h>
#include <Object/Enemy/Enemy.h>

#include <Math/Utility/Easing.h>
#include <Math/Utility/MakeMatrixMath.h>

#include "algorithm"

using namespace MyEngine;

///=====================================================/// 
/// ロックオンシステムを初期化
///=====================================================///
void LockOn::Initialize(EngineContext context, Player* playerPtr) {

	//引数の取得
	camera_ = context.camera;

	player_ = playerPtr;

	//メインレティクルの生成
	mainReticle_ = std::make_unique<Reticle>();

	mainReticle_->Initialize(context);

	//プレイヤーからのオフセットを設定
	offset_ = { 0.0f,0.0f,30.0f };

	//移動範囲を設定
	moveRange_ = { 7.0f,5.0f,0.0f };

	//移動強度を設定
	moveStrength_ = 5.0f;

	//最大ロックオン数を設定
	maxLockOnNum_ = 5;

	//ロックオンレティクルを生成
	for (int i = 0; i < maxLockOnNum_; i++) {

		//ロックオンレティクルの生成
		std::unique_ptr<LockOnReticle> lockOnReticle = std::make_unique<LockOnReticle>();

		//構成要素の初期化

		//レティクルの生成
		lockOnReticle->reticle = std::make_unique<Reticle>();

		lockOnReticle->reticle->Initialize(context);

		//ターゲットの初期化
		lockOnReticle->targetEnemy = nullptr;

		//前回ターゲットの初期化
		lockOnReticle->preTargetEnemy = nullptr;

		//アクティブフラグの初期化
		lockOnReticle->isActive = false;

		//リストに追加
		lockOnReticles_.emplace_back(std::move(lockOnReticle));
	}
}

///=====================================================/// 
/// ロックオンシステムを更新
///=====================================================///
void LockOn::Update() {

	/// === メインレティクルの更新 === ///

	//プレイヤーの座標
	Vector3 playerPos = player_->GetWorldPos();

	//プレイヤーの移動方向
	Vector3 moveDirection = Normalize(player_->GetInputDirection());

	//メインレティクルの座標
	Vector3 mainReticlePos = mainReticle_->GetWorldPos();

	//プレイヤーが動いていれば
	if (Length(moveDirection) != 0.0f) {

		//プレイヤーの現在位置からオフセット分移動した座標
		Vector3 offsetPos = player_->GetWorldPos() + offset_;

		//移動後の座標
		Vector3 movePos = offsetPos + (moveDirection * Length(moveRange_));

		//制限範囲からでないようにする
		movePos = {
			std::clamp(movePos.x, playerPos.x - moveRange_.x, playerPos.x + moveRange_.x),
			std::clamp(movePos.y, playerPos.y - moveRange_.y, playerPos.y + moveRange_.y),
			movePos.z,
		};

		//線形補間で移動
		mainReticlePos = Lerp(mainReticlePos, movePos, moveStrength_ / 100.0f);
	}

	//地面の中に埋まらないようにする
	mainReticlePos.y = fmaxf(1.0f, mainReticlePos.y);

	//メインレティクルのターゲット座標を設定
	mainReticle_->SetTargetPos(mainReticlePos);

	//メインレティクルの更新
	mainReticle_->Update();

	/// === ロックオンレティクルの更新 === ///

	//前回と同じ敵が割り当てられるなら優先的に割り当てる
	for (auto& lockOnReticle : lockOnReticles_) {

		//アクティブフラグの初期化
		lockOnReticle->isActive = false;

		for(auto& enemy : lockedEnemies_) {

			if (enemy.enemy->GetIsDead()) {
				continue;
			}

			//前回と同じ敵なら
			if (lockOnReticle->preTargetEnemy == enemy.enemy) {

				//ターゲットに敵を設定
				lockOnReticle->targetEnemy = enemy.enemy;

				//ターゲットの有効化
				enemy.isAssigned = true;
			}
		}
	}

	//未割り当ての敵を割り当てる
	for(auto& enemy : lockedEnemies_) {

		if (enemy.enemy->GetIsDead()) {
			continue;
		}

		if (!enemy.isAssigned) {

			for(auto& lockOnReticle : lockOnReticles_) {

				//ターゲットが未設定なら
				if (lockOnReticle->targetEnemy == nullptr) {

					//ターゲットに敵を設定
					lockOnReticle->targetEnemy = enemy.enemy;

					//アニメーションタイマーをリセット
					lockOnReticle->reticle->ResetAnimTimer();

					//ターゲットの有効化
					enemy.isAssigned = true;

					break;
				}
			}
		}
	}

	//ロックオンレティクルの更新
	for (auto& lockOnReticle : lockOnReticles_) {

		//ターゲットが存在すれば
		if (lockOnReticle->targetEnemy != nullptr) {

			//ターゲットの座標
			Vector3 targetPos = lockOnReticle->targetEnemy->GetWorldPos();

			//ロックオンレティクルのターゲット座標を設定
			lockOnReticle->reticle->SetTargetPos(targetPos);

			//アクティブフラグを立てる
			lockOnReticle->isActive = true;
		}

		//前回ターゲットの更新
		lockOnReticle->preTargetEnemy = lockOnReticle->targetEnemy;

		//ターゲットのリセット
		lockOnReticle->targetEnemy = nullptr;

		//ロックオンレティクルの更新
		lockOnReticle->reticle->Update();
	}

	//ロックオン敵リストをクリア
	lockedEnemies_.clear();
}

///=====================================================/// 
/// メインおよびロックオンレティクルを描画
///=====================================================///
void LockOn::Draw() {

	//メインレティクルの描画
	mainReticle_->Draw();

	//ロックオンレティクルの描画
	for(auto& lockOnReticle : lockOnReticles_) {

		//アクティブなら描画
		if (lockOnReticle->isActive) {

			lockOnReticle->reticle->Draw();
		}
	}
}

///=====================================================/// 
/// 指定した敵をロックオン対象に追加
///=====================================================///
void LockOn::AddLockOnEnemy(Enemy* target) {

	//既にロックされているなら何もしない
	for(auto& enemy : lockedEnemies_) {
		if (enemy.enemy == target) {
			return;
		}
	}

	LockedEnemy newLockedEnemy;

	newLockedEnemy.enemy = target;
	newLockedEnemy.isAssigned = false;

	//ターゲット情報をリストに追加
	lockedEnemies_.emplace_back(newLockedEnemy);
}

///=====================================================/// 
/// アクティブなロックオンレティクルの座標を取得
///=====================================================///
std::vector<Vector3> LockOn::GetLockOnReticlePos() {

	std::vector<Vector3> reticlePositions;

	//アクティブなロックオンレティクルの座標を取得
	for (auto& lockOnReticle : lockOnReticles_) {

		if (lockOnReticle->isActive) {

			reticlePositions.emplace_back(lockOnReticle->reticle->GetWorldPos());
		}
	}

	return reticlePositions;
}

float LockOn::GetMainReticleToEnemyLength(Enemy* enemy) {

	//敵のスクリーン座標
	Vector3 enemyScreenPos = Vector3ToScreenSpace(camera_, enemy->GetWorldPos());

	//メインレティクルのスクリーン座標
	Vector3 mainReticleScreenPos = Vector3ToScreenSpace(camera_, mainReticle_->GetWorldPos());

	//レティクルとエネミーの距離を返す
	return Length(enemyScreenPos - mainReticleScreenPos);
}