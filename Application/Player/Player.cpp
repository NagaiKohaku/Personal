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
#include "imgui.h"

///=====================================================/// 
/// プレイヤーを初期化
///=====================================================///
void Player::Initialize(Camera* cameraPtr, BulletManager* bulletPtr, bool isMoveActive) {

	//カメラポインタを取得
	camera_ = cameraPtr;

	//バレットマネージャーを取得
	bulletManager_ = bulletPtr;

	ModelManager::GetInstance()->LoadModel("Player", "Player", "Core");

	/// === オブジェクトの生成 === ///

	//コアオブジェクトの生成
	core_ = std::make_unique<Object3D>();

	core_->Initialize();

	core_->SetModel("Player");

	rightWingTransform_ = std::make_unique<WorldTransform>();

	rightWingTransform_->Initialize();

	//rightWingTransform_->SetParent(&core_->GetWorldTransform());

	leftWingTransform_ = std::make_unique<WorldTransform>();

	leftWingTransform_->Initialize();

	//leftWingTransform_->SetParent(&core_->GetWorldTransform());

	//右トレイルエミッターの生成
	rightTrail_ = std::make_unique<EmitterGroup>();

	rightTrail_->Initialize(camera_);

	rightTrail_->LoadEmitter("Trail");

	rightTrail_->GetWorldTransform().SetParent(&core_->GetWorldTransform());

	rightTrail_->Emit();

	//左トレイルエミッターの生成
	leftTrail_ = std::make_unique<EmitterGroup>();

	leftTrail_->Initialize(camera_);

	leftTrail_->LoadEmitter("Trail");

	leftTrail_->GetWorldTransform().SetParent(&core_->GetWorldTransform());

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

	/// === 他変数の設定 === ///

	//攻撃タイマーの設定
	attackTimer_ = 0.0f;

	//戦車状態の攻撃間隔
	tankAttackInterval_ = 1.0f;

	//飛行機状態の攻撃間隔
	jetAttackInterval_ = 0.1f;

	//移動速度の設定
	moveSpeed_ = 1.0f;

	//移動強度の設定
	moveStrength_ = 10.0f;

	//回転強度の設定
	rotStrength_ = 10.0f;

	//ロックオン範囲の設定
	lockOnRange_ = 300.0f;

	//移動範囲の設定
	moveRange_ = { 7.0f,4.0f,0.0f };

	//戦車状態の回転範囲の設定
	driveRotRange_ = { 0.0f,0.5f,0.0f };

	//飛行機状態の回転範囲の設定
	flightRotRange_ = { 0.5f,0.0f,0.3f };

	tankWingPosOffset_ = { 1.0f,-1.4f,0.0f };

	tankWingRotOffset_ = { std::numbers::pi_v<float> *2.0f,0.0f,std::numbers::pi_v<float> *0.5f };

	jetWingPosOffset_ = { 0.0f,0.0f,0.0f };

	jetWingRotOffset_ = { 0.0f,0.0f,0.0f };

	//初期座標の設定
	initialPos_ = { 0.0f,2.0f,0.0f };

	//座標の設定
	core_->GetWorldTransform().translate_ = initialPos_;

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

	rightWingTransform_->UpdateMatrix();

	leftWingTransform_->UpdateMatrix();

	core_->GetModel()->SetSubmeshLocalTransform(static_cast<size_t>(1), rightWingTransform_->GetWorldMatrix());

	core_->GetModel()->SetSubmeshLocalTransform(static_cast<size_t>(2), leftWingTransform_->GetWorldMatrix());

	core_->GetModel()->UpdateSubmeshTransformsCPU();

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

	rightWingTransform_->UpdateMatrix();

	leftWingTransform_->UpdateMatrix();

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

///=====================================================/// 
/// プレイヤーの移動処理
///=====================================================///
void Player::Move() {

	//移動量をリセット
	velocity_ = { 0.0f,0.0f,0.0f };

	if (isMoveActive_) {

		//Wキーが押されたら上方向に移動
		if (Input::GetInstance()->isPushKey(DIK_W)) {

			velocity_.y += moveSpeed_;
		}

		//Sキーが押されたら下方向に移動
		if (Input::GetInstance()->isPushKey(DIK_S)) {

			velocity_.y -= moveSpeed_;
		}

		//Aキーが押されたら左方向に移動
		if (Input::GetInstance()->isPushKey(DIK_A)) {

			velocity_.x -= moveSpeed_;
		}

		//Dキーが押されたら右方向に移動
		if (Input::GetInstance()->isPushKey(DIK_D)) {

			velocity_.x += moveSpeed_;
		}
	}

	//移動量の長さが0でなければ
	if (Length(velocity_) != 0.0f) {

		//プレイヤーの現在座標
		Vector3 playerPos = core_->GetWorldTransform().translate_;

		//移動後の座標
		Vector3 movePos = playerPos + velocity_;

		//初期座標の移動範囲からはみ出さないように移動座標を制限
		movePos = {
			std::clamp(movePos.x,initialPos_.x - moveRange_.x,initialPos_.x + moveRange_.x),
			std::clamp(movePos.y,initialPos_.y - moveRange_.y,initialPos_.y + moveRange_.y),
			std::clamp(movePos.z,initialPos_.z - moveRange_.z,initialPos_.z + moveRange_.z),
		};

		//線形補間で移動後座標に向かって移動
		Vector3 moveResult = Lerp(playerPos, movePos, moveStrength_ / 100.0f);

		//地面よりも下に行かないようにする
		moveResult.y = fmaxf(1.0f, moveResult.y);

		//オブジェクトの座標を結果座標で設定
		core_->GetWorldTransform().translate_ = moveResult;
	}

	switch (moveState_) {
	case Player::TANK:

		//戦車状態の移動
		TankMove();

		break;
	case Player::JET:

		//飛行機状態の移動
		JetMove();

		break;
	}

}

///=====================================================/// 
/// 戦車モード時のプレイヤーの移動およびパーツ演出処理
///=====================================================///
void Player::TankMove() {

	//プレイヤーの現在角度
	Vector3 playerRot = core_->GetWorldTransform().rotate_;

	//右トレイルの現在座標
	Vector3 rightTrailPos = rightTrail_->GetWorldTransform().translate_;

	//左トレイルの現在座標
	Vector3 leftTrailPos = leftTrail_->GetWorldTransform().translate_;

	float pi = std::numbers::pi_v<float>;

	//左右移動に応じてY軸回転をするように設定
	Vector3 rotate = {
		0.0f,
		Normalize(velocity_).x,
		0.0f
	};

	//回転範囲で制限
	rotate = {
		std::clamp(rotate.x,-driveRotRange_.x,driveRotRange_.x),
		std::clamp(rotate.y,-driveRotRange_.y,driveRotRange_.y),
		std::clamp(rotate.z,-driveRotRange_.z,driveRotRange_.z)
	};

	//コアオブジェクトの回転
	core_->GetWorldTransform().rotate_ = Lerp(playerRot, rotate, rotStrength_ / 100.0f);

	rightWingTransform_->translate_ = EaseOut(
		rightWingTransform_->translate_,
		Vector3(
			tankWingPosOffset_.x,
			tankWingPosOffset_.y,
			tankWingPosOffset_.z
		),
		0.1f,
		2.0f
	);

	rightWingTransform_->rotate_ = EaseOut(
		rightWingTransform_->rotate_,
		Vector3(
			tankWingRotOffset_.x,
			tankWingRotOffset_.y,
			tankWingRotOffset_.z
		),
		0.1f,
		2.0f
	);

	leftWingTransform_->translate_ = EaseOut(
		leftWingTransform_->translate_,
		Vector3(
			-tankWingPosOffset_.x,
			tankWingPosOffset_.y,
			tankWingPosOffset_.z
		),
		0.1f,
		2.0f
	);

	leftWingTransform_->rotate_ = EaseOut(
		leftWingTransform_->rotate_,
		Vector3(
			tankWingRotOffset_.x,
			tankWingRotOffset_.y,
			-tankWingRotOffset_.z
		),
		0.1f,
		2.0f
	);

	Vector3 rightTrailEasePos = Vector3(1.0f, 0.0f, 0.0f) - Vector3(0.0f, 1.0f, 0.0f);

	Vector3 leftTrailEasePos = Vector3(-1.0f, 0.0f, 0.0f) - Vector3(0.0f, 1.0f, 0.0f);

	//右トレイルの移動
	rightTrail_->GetWorldTransform().translate_ = EaseOut(rightTrailPos, rightTrailEasePos, 0.1f, 2.0f);

	//左トレイルの移動
	leftTrail_->GetWorldTransform().translate_ = EaseOut(leftTrailPos, leftTrailEasePos, 0.1f, 2.0f);
}

///=====================================================/// 
/// 飛行機モード時のプレイヤーの移動およびパーツ演出処理
///=====================================================///
void Player::JetMove() {

	Vector3 playerPos = core_->GetWorldTransform().translate_;

	//コアオブジェクトの現在角度
	Vector3 playerRot = core_->GetWorldTransform().rotate_;

	//右トレイルの現在座標
	Vector3 rightTrailPos = rightTrail_->GetWorldTransform().translate_;

	//左トレイルの現在座標
	Vector3 leftTrailPos = leftTrail_->GetWorldTransform().translate_;

	//左右移動でZ軸回転、上下移動でX軸回転をするように設定
	Vector3 rotate = {
		-Normalize(velocity_).y,
		0.0f,
		-Normalize(velocity_).x
	};

	//回転範囲で制限
	rotate = {
		std::clamp(rotate.x,-flightRotRange_.x,flightRotRange_.x),
		std::clamp(rotate.y,-flightRotRange_.y,flightRotRange_.y),
		std::clamp(rotate.z,-flightRotRange_.z,flightRotRange_.z)
	};

	//コアオブジェクトの回転
	core_->GetWorldTransform().rotate_ = Lerp(playerRot, rotate, rotStrength_ / 100.0f);

	rightWingTransform_->translate_ = EaseOut(
		rightWingTransform_->translate_,
		Vector3(
			jetWingPosOffset_.x,
			jetWingPosOffset_.y,
			jetWingPosOffset_.z
		),
		0.1f,
		2.0f
	);

	rightWingTransform_->rotate_ = EaseOut(
		rightWingTransform_->rotate_,
		Vector3(
			jetWingRotOffset_.x,
			jetWingRotOffset_.y,
			jetWingRotOffset_.z
		),
		0.1f,
		2.0f
	);

	leftWingTransform_->translate_ = EaseOut(
		leftWingTransform_->translate_,
		Vector3(
			-jetWingPosOffset_.x,
			jetWingPosOffset_.y,
			jetWingPosOffset_.z
		),
		0.1f,
		2.0f
	);

	leftWingTransform_->rotate_ = EaseOut(
		leftWingTransform_->rotate_,
		Vector3(
			jetWingRotOffset_.x,
			jetWingRotOffset_.y,
			-jetWingRotOffset_.z
		),
		0.1f,
		2.0f
	);

	Vector3 rightTrailEasePos = Vector3(2.0f, 0.0f, 0.0f);

	Vector3 leftTrailEasePos = Vector3(-2.0f, 0.0f, 0.0f);

	//右トレイルの移動
	rightTrail_->GetWorldTransform().translate_ = EaseOut(rightTrailPos, rightTrailEasePos, 0.1f, 2.0f);

	//左トレイルの移動
	leftTrail_->GetWorldTransform().translate_ = EaseOut(leftTrailPos, leftTrailEasePos, 0.1f, 2.0f);
}

///=====================================================/// 
/// プレイヤーの攻撃処理
///=====================================================///
void Player::Attack() {

	//攻撃タイマーを進める
	attackTimer_ += 1.0f / 60.0f;

	switch (moveState_) {
	case Player::TANK:

		//戦車状態の攻撃
		TankAttack();

		break;
	case Player::JET:

		//飛行機状態の攻撃
		JetAttack();

		break;
	}
}

///=====================================================/// 
/// 戦車モード時の攻撃処理
///=====================================================///
void Player::TankAttack() {

	std::list<Enemy*> enemyList = ObjectManager::GetInstance()->GetEnemies();

	//タイマーが攻撃間隔を超えたら
	if (attackTimer_ >= tankAttackInterval_) {

		for (auto& enemy : enemyList) {

			//敵が死亡していたらスキップ
			if (enemy->CheckIsDead()) {

				continue;
			}

			//敵のスクリーン座標
			Vector3 enemyScreenPos = Vector3ToScreenSpace(camera_, enemy->GetWorldPos());

			//メインレティクルのスクリーン座標
			Vector3 mainReticleScreenPos = Vector3ToScreenSpace(camera_, lockOn_->GetMainReticlePos());

			//ロックオン範囲内であれば
			if (Length(enemyScreenPos - mainReticleScreenPos) <= lockOnRange_) {

				//ロックオン対象に追加
				lockOn_->AddLockOnEnemy(enemy);
			}
		}

		//スペースキーが押されていたら
		if (Input::GetInstance()->isPushKey(DIK_SPACE)) {

			//マズルフラッシュエミッターを発生
			muzzleFlashEmitter_->Emit();

			//レティクルの位置を取得
			std::vector<Vector3> reticlePositions = lockOn_->GetLockOnReticlePos();

			for (auto& pos : reticlePositions) {

				//オブジェクトからレティクルへの方向
				Vector3 direction = pos - core_->GetWorldTransform().GetWorldTranslate();

				//バレットマネージャーに弾を追加
				bulletManager_->AddBullet(
					core_->GetWorldTransform().translate_,
					Normalize(direction),
					BulletManager::BULLETTYPE::TANK
				);
			}

			//攻撃のタイマーをリセット
			attackTimer_ = 0.0f;
		}
	}
}

///=====================================================/// 
/// 飛行機モード時の攻撃処理
///=====================================================///
void Player::JetAttack() {

	//タイマーが攻撃間隔を超えたら
	if (attackTimer_ >= jetAttackInterval_) {

		//スペースキーが押されていたら
		if (Input::GetInstance()->isPushKey(DIK_SPACE)) {

			//マズルフラッシュエミッターを発生
			muzzleFlashEmitter_->Emit();

			//オブジェクトからレティクルへの方向
			Vector3 direction = lockOn_->GetMainReticlePos() - core_->GetWorldTransform().GetWorldTranslate();

			//バレットマネージャーに弾を追加
			bulletManager_->AddBullet(
				core_->GetWorldTransform().translate_,
				Normalize(direction),
				BulletManager::BULLETTYPE::JET
			);

			//攻撃のタイマーをリセット
			attackTimer_ = 0.0f;
		}
	}
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

void Player::ImGui() {

}