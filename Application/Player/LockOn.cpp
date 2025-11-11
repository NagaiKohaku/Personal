#include "LockOn.h"

#include "3d/Camera/Camera.h"
#include "Player/Player.h"
#include "Enemy/Enemy.h"

#include "Math/Easing.h"

#include "algorithm"

void LockOn::Initialize(Camera* cameraPtr, Player* playerPtr) {

	camera_ = cameraPtr;

	player_ = playerPtr;

	//メインレティクルの生成

	mainReticle_ = std::make_unique<Reticle>();

	mainReticle_->Initialize(camera_);

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

		std::unique_ptr<LockOnReticle> lockOnReticle = std::make_unique<LockOnReticle>();

		lockOnReticle->reticle = std::make_unique<Reticle>();
		lockOnReticle->reticle->Initialize(camera_);
		lockOnReticle->targetEnemy = nullptr;
		lockOnReticle->preTargetEnemy = nullptr;
		lockOnReticle->isActive = false;

		lockOnReticles_.emplace_back(std::move(lockOnReticle));
	}
}

void LockOn::Update() {

	/// === メインレティクルの更新 === ///

	//プレイヤーの座標
	Vector3 playerPos = player_->GetWorldPos();

	//プレイヤーの移動方向
	Vector3 moveDirection = Normalize(player_->GetVelocity());

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

		lockOnReticle->isActive = false;

		for(auto& enemy : lockedEnemies_) {

			if (lockOnReticle->preTargetEnemy == enemy.enemy) {

				lockOnReticle->targetEnemy = enemy.enemy;

				enemy.isAssigned = true;
			}
		}
	}

	for(auto& enemy : lockedEnemies_) {

		if (!enemy.isAssigned) {

			for(auto& lockOnReticle : lockOnReticles_) {

				if (lockOnReticle->targetEnemy == nullptr) {

					lockOnReticle->targetEnemy = enemy.enemy;

					lockOnReticle->reticle->ResetAnimTimer();

					enemy.isAssigned = true;

					break;
				}
			}
		}
	}

	//ロックオンレティクルの更新
	for (auto& lockOnReticle : lockOnReticles_) {

		if (lockOnReticle->targetEnemy != nullptr) {

			//ターゲット敵の座標
			Vector3 targetPos = lockOnReticle->targetEnemy->GetWorldPos();

			//ロックオンレティクルのターゲット座標を設定
			lockOnReticle->reticle->SetTargetPos(targetPos);

			lockOnReticle->isActive = true;
		}

		lockOnReticle->preTargetEnemy = lockOnReticle->targetEnemy;

		lockOnReticle->targetEnemy = nullptr;

		//ロックオンレティクルの更新
		lockOnReticle->reticle->Update();
	}

	//ロックオン敵リストをクリア
	lockedEnemies_.clear();
}

void LockOn::Draw() {

	mainReticle_->Draw();

	for(auto& lockOnReticle : lockOnReticles_) {

		if (lockOnReticle->isActive) {

			lockOnReticle->reticle->Draw();
		}
	}
}

void LockOn::AddLockOnEnemy(Enemy* target) {

	for(auto& enemy : lockedEnemies_) {
		if (enemy.enemy == target) {
			return;
		}
	}

	LockedEnemy newLockedEnemy;

	newLockedEnemy.enemy = target;
	newLockedEnemy.isAssigned = false;

	lockedEnemies_.emplace_back(newLockedEnemy);
}

std::vector<Vector3> LockOn::GetLockOnReticlePos() {

	std::vector<Vector3> reticlePositions;

	for (auto& lockOnReticle : lockOnReticles_) {

		if (lockOnReticle->isActive) {

			reticlePositions.emplace_back(lockOnReticle->reticle->GetWorldPos());
		}
	}

	return reticlePositions;
}