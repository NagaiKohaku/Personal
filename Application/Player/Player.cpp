#include "Player.h"

#include "3d/Model/ModelManager.h"

#include "Base/Input.h"

#include "LevelEditor/LevelDataLoader.h"
#include "ObjectManager.h"
#include "Bullet/BulletManager.h"

#include "Enemy/Enemy.h"

#include "Shake/Shake.h"

#include "Math/Easing.h"
#include "Math/MakeMatrixMath.h"

#include "algorithm"
#include "numbers"

///=====================================================/// 
/// プレイヤーを初期化
///=====================================================///
void Player::Initialize(Camera* cameraPtr, BulletManager* bulletPtr, bool isMoveActive) {

	//カメラポインタを取得
	camera_ = cameraPtr;

	//バレットマネージャーを取得
	bulletManager_ = bulletPtr;

	//レベルデータローダーを取得
	levelDataLoader_ = LevelDataLoader::GetInstance();

	//オブジェクトデータを読み込み
	objectData_ = levelDataLoader_->PickObjectData("PlayerObject/JetPlayer.json", ObjectType::PLAYER);

	/// === オブジェクトの生成 === ///

	//コアオブジェクトの生成
	core_ = std::make_unique<Object3D>();

	core_->Initialize(objectData_[0]);

	//右ウィングオブジェクトの生成
	rightWing_ = std::make_unique<Object3D>();

	rightWing_->Initialize(objectData_[1]);

	rightWing_->GetWorldTransform().SetParent(&core_->GetWorldTransform());

	//左ウィングオブジェクトの生成
	leftWing_ = std::make_unique<Object3D>();

	leftWing_->Initialize(objectData_[2]);

	leftWing_->GetWorldTransform().SetParent(&core_->GetWorldTransform());

	//右トレイルエミッターの生成
	rightTrail_ = std::make_unique<EmitterGroup>();

	rightTrail_->Initialize(camera_);

	rightTrail_->LoadEmitter("Trail");

	rightTrail_->GetWorldTransform().SetParent(&rightWing_->GetWorldTransform());

	rightTrail_->Emit();

	//左トレイルエミッターの生成
	leftTrail_ = std::make_unique<EmitterGroup>();

	leftTrail_->Initialize(camera_);

	leftTrail_->LoadEmitter("Trail");

	leftTrail_->GetWorldTransform().SetParent(&leftWing_->GetWorldTransform());

	leftTrail_->Emit();

	//死亡時エミッターの生成
	explosiveEmitter_ = std::make_unique<EmitterGroup>();

	explosiveEmitter_->Initialize(camera_);

	explosiveEmitter_->LoadEmitter("PlayerExplosive");

	explosiveEmitter_->GetWorldTransform().SetParent(&core_->GetWorldTransform());

	//破壊時エミッターの生成
	destroyEmitter_ = std::make_unique<EmitterGroup>();

	destroyEmitter_->Initialize(camera_);

	destroyEmitter_->LoadEmitter("Destroy");

	destroyEmitter_->GetWorldTransform().SetParent(&core_->GetWorldTransform());

	//マズルフラッシュエミッターの生成
	muzzleFlashEmitter_ = std::make_unique<EmitterGroup>();

	muzzleFlashEmitter_->Initialize(camera_);

	muzzleFlashEmitter_->LoadEmitter("MuzzleFlash");

	muzzleFlashEmitter_->GetWorldTransform().SetParent(&core_->GetWorldTransform());

	//影オブジェクトの生成
	shadow_ = std::make_unique<Shadow>();

	shadow_->Initialize();

	/// === コライダーの生成 === ///

	//生成
	collider_ = std::make_unique<SphereCollider>();

	//初期化
	collider_->Initialize(&core_->GetWorldTransform());

	//タグの設定
	collider_->SetTag(Collider::Tag::PLAYER);

	//大きさの設定
	collider_->SetRadius(0.5f);

	/// === レティクルの生成 === ///

	//生成
	lockOn_ = std::make_unique<LockOn>();

	//初期化
	lockOn_->Initialize(camera_, this);

	/// === 移動ステートの初期化 === ///

	movementState_ = std::make_unique<TankMoveState>();

	movementState_->Enter();

	/// === 攻撃ステートの初期化 === ///

	attackState_ = std::make_unique<TankAttackState>();

	attackState_->Enter();

	/// === 他変数の設定 === ///

	//移動アクティブフラグの設定
	isMoveActive_ = isMoveActive;

	//死亡フラグの設定
	isDead_ = false;
}

///=====================================================/// 
/// プレイヤーの状態を更新
///=====================================================///
void Player::Update() {

	//すでに死亡していれば処理なし
	if (!isDead_) {

		//地面に接していたら
		if (core_->GetWorldTransform().translate_.y <= 1.0f) {

			//戦車状態に変更
			moveState_ = TANK;
		} else {

			//飛行機状態に変更
			moveState_ = JET;
		}

		//移動
		Move();
	}

	//移動可能であれば
	if (isMoveActive_) {

		//攻撃
		Attack();

		//衝突判定
		IsCollision();
	}

	//死亡していれば
	if (isDead_) {

		//死亡処理
		Dead();
	}

	//プレイヤーの更新
	core_->Update();

	//右ウィングの更新
	rightWing_->Update();

	//左ウィングの更新
	leftWing_->Update();

	//右トレイルの更新
	rightTrail_->Update();

	//左トレイルの更新
	leftTrail_->Update();

	//死亡時爆発エミッターの更新
	explosiveEmitter_->Update();

	//破壊エミッターの更新
	destroyEmitter_->Update();

	//マズルフラッシュエミッターの更新
	muzzleFlashEmitter_->Update();

	//影の更新
	shadow_->Update(core_->GetWorldTransform().translate_);

	//移動可能であれば
	if (isMoveActive_) {

		//コライダーの更新
		collider_->Update();

		//レティクルの更新
		lockOn_->Update();
	}
}

///=====================================================/// 
/// プレイヤーの座標のみを更新
///=====================================================///
void Player::TransformUpdate() {

	//コアオブジェクトの更新
	core_->Update();

	//右ウィングの更新
	rightWing_->Update();

	//左ウィングの更新
	leftWing_->Update();

	//影の更新
	shadow_->Update(core_->GetWorldTransform().translate_);
}

///=====================================================/// 
/// プレイヤーの各種描画処理
///=====================================================///
void Player::Draw() {

	//破壊されていなければ
	if (!isDestroy_) {

		//コアオブジェクトの描画
		core_->Draw(LayerType::Object);

		//右ウィングの描画
		rightWing_->Draw(LayerType::Object);

		//左ウィングの描画
		leftWing_->Draw(LayerType::Object);

		//影の描画
		shadow_->Draw();
	}

	//右トレイルの描画
	rightTrail_->Draw();

	//左トレイルの描画
	leftTrail_->Draw();

	//死亡時爆発エミッターの描画
	explosiveEmitter_->Draw();

	//破壊エミッターの描画
	destroyEmitter_->Draw();

	//マズルフラッシュエミッターの描画
	muzzleFlashEmitter_->Draw();

	//移動可能であれば
	if (isMoveActive_) {

		//コライダーの描画
		collider_->Draw();

		//レティクルの描画
		lockOn_->Draw();
	}
}

void Player::UpdateLockOn(float lockOnRange) {

	std::list<Enemy*> enemyList = ObjectManager::GetInstance()->GetEnemies();

	for (auto& enemy : enemyList) {

		//敵が死亡していたらスキップ
		if (enemy->CheckIsDead()) {

			continue;
		}

		//ロックオン範囲内であれば
		if (lockOn_->GetMainReticleToEnemyLength(enemy) <= lockOnRange) {

			//ロックオン対象に追加
			lockOn_->AddLockOnEnemy(enemy);
		}
	}
}

void Player::JetAttack() {

	//マズルフラッシュエミッターを発生
	muzzleFlashEmitter_->Emit();

	//オブジェクトからレティクルへの方向
	Vector3 direction = lockOn_->GetMainReticlePos() - core_->GetWorldTransform().translate_;

	//バレットマネージャーに弾を追加
	bulletManager_->AddBullet(
		core_->GetWorldTransform().translate_,
		Normalize(direction),
		BulletManager::BULLETTYPE::JET
	);

}

void Player::TankAttack() {

	//マズルフラッシュエミッターを発生
	muzzleFlashEmitter_->Emit();

	//レティクルの位置を取得
	std::vector<Vector3> reticlePositions = lockOn_->GetLockOnReticlePos();

	for (auto& pos : reticlePositions) {

		//オブジェクトからレティクルへの方向
		Vector3 direction = pos - core_->GetWorldTransform().translate_;

		//バレットマネージャーに弾を追加
		bulletManager_->AddBullet(
			core_->GetWorldTransform().translate_,
			Normalize(direction),
			BulletManager::BULLETTYPE::TANK
		);
	}

}

void Player::ChangeJetState() {

	movementState_ = std::make_unique<JetMoveState>();
	movementState_->Enter();

	attackState_ = std::make_unique<JetAttackState>();
	attackState_->Enter();
}

void Player::ChangeTankState() {

	movementState_ = std::make_unique<TankMoveState>();
	movementState_->Enter();

	attackState_ = std::make_unique<TankAttackState>();
	attackState_->Enter();
}

///=====================================================/// 
/// プレイヤーの移動処理
///=====================================================///
void Player::Move() {

	//移動量をリセット
	inputDirection_ = { 0.0f,0.0f,0.0f };

	if (isMoveActive_) {

		//Wキーが押されたら上方向に移動
		if (Input::GetInstance()->isPushKey(DIK_W)) {

			inputDirection_.y += 1.0f;
		}

		//Sキーが押されたら下方向に移動
		if (Input::GetInstance()->isPushKey(DIK_S)) {

			inputDirection_.y -= 1.0f;
		}

		//Aキーが押されたら左方向に移動
		if (Input::GetInstance()->isPushKey(DIK_A)) {

			inputDirection_.x -= 1.0f;
		}

		//Dキーが押されたら右方向に移動
		if (Input::GetInstance()->isPushKey(DIK_D)) {

			inputDirection_.x += 1.0f;
		}
	}

	movementState_->Update(*this);
}

///=====================================================/// 
/// プレイヤーの攻撃処理
///=====================================================///
void Player::Attack() {

	attackState_->Update(*this);
}

///=====================================================/// 
/// プレイヤーと他オブジェクトの衝突判定
///=====================================================///
void Player::IsCollision() {

	//接触状態であれば
	if (collider_->GetIsTrigger()) {

		//接触相手のタグがPLAYERBULLETであれば
		if (collider_->CheckHitTag(Collider::Tag::ENEMYBULLET)) {

			//移動不可にする
			isMoveActive_ = false;

			//死亡フラグを立てる
			isDead_ = true;

			//死亡時爆発エミッターを発生
			explosiveEmitter_->Emit();
		}
	}

}

///=====================================================/// 
/// プレイヤーが死亡した際の処理
///=====================================================///
void Player::Dead() {

	//下に移動・回転
	core_->GetWorldTransform().translate_ += Vector3(0.0f, -0.03f, 0.0f);

	//コアを軸に回転
	core_->GetWorldTransform().rotate_ += Vector3(0.0f, 0.0f, 0.1f);

	//地面に接したら
	if (core_->GetWorldTransform().translate_.y <= 1.0f) {

		//地面に接した位置で固定
		core_->GetWorldTransform().translate_.y = 1.0f;

		//破壊されていなければ
		if (!isDestroy_) {

			//破壊フラグを立てる
			isDestroy_ = true;

			//トレイル・爆発エミッター停止
			leftTrail_->Stop();

			rightTrail_->Stop();

			explosiveEmitter_->Stop();

			destroyEmitter_->Emit();

			//画面揺れ開始
			Shake::GetInstance()->Start(1.0f, 0.5f);
		}
	}
}