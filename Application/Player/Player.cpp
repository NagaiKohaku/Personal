#include "Player.h"

#include "Bullet/BulletManager.h"

#include "Base/Input.h"

#include "Math/Easing.h"

#include "algorithm"

///=====================================================/// 
/// 初期化
///=====================================================///
void Player::Initialize(Camera* cameraPtr, BulletManager* bulletPtr) {

	//カメラポインタを取得
	camera_ = cameraPtr;

	//バレットマネージャーを取得
	bulletManager_ = bulletPtr;

	/// === オブジェクトの生成 === ///

	//生成
	object_ = std::make_unique<Object3D>();

	//初期化
	object_->Initialize();

	//モデルの設定
	object_->SetModel("Cube");

	/// === コライダーの生成 === ///

	//生成
	collider_ = std::make_unique<SphereCollider>();

	//初期化
	collider_->Initialize(&object_->GetWorldTransform());

	//タグの設定
	collider_->SetTag(Collider::Tag::PLAYER);

	//大きさの設定
	collider_->SetRadius(1.5f);

	/// === レティクルの生成 === ///

	//生成
	reticle_ = std::make_unique<Reticle>();

	//初期化
	reticle_->Initialize(camera_, this);

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

	//移動範囲の設定
	moveRange_ = { 7.0f,4.0f,0.0f };

	//戦車状態の回転範囲の設定
	driveRotRange_ = { 0.0f,0.5f,0.0f };

	//飛行機状態の回転範囲の設定
	flightRotRange_ = { 0.2f,0.0f,0.2f };

	//初期座標の設定
	initialPos_ = { 0.0f,2.0f,0.0f };

	//座標の設定
	object_->GetWorldTransform().translate_ = initialPos_;

}

///=====================================================/// 
/// 更新
///=====================================================///
void Player::Update() {

	//地面に接していたら
	if (object_->GetWorldTransform().translate_.y == 1.0f) {

		//戦車状態に変更
		moveState_ = TANK;
	} else {

		//飛行機状態に変更
		moveState_ = JET;
	}

	//移動
	Move();

	//攻撃
	Attack();

	//衝突判定
	IsCollision();

	//プレイヤーの更新
	object_->Update();

	//コライダーの更新
	collider_->Update();

	reticle_->Update();
}

///=====================================================/// 
/// 描画
///=====================================================///
void Player::Draw() {

	//プレイヤーの描画
	object_->Draw(LayerType::Object);

	//コライダーの描画
	collider_->Draw();

	//レティクルの描画
	reticle_->Draw();
}

///=====================================================/// 
/// 移動
///=====================================================///
void Player::Move() {

	//移動量をリセット
	velocity_ = { 0.0f,0.0f,0.0f };

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

	//移動量の長さが0でなければ
	if (Length(velocity_) != 0.0f) {

		//プレイヤーの現在座標
		Vector3 playerPos = object_->GetWorldTransform().translate_;

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
		object_->GetWorldTransform().translate_ = moveResult;
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
/// 戦車状態の移動
///=====================================================///
void Player::TankMove() {

	//プレイヤーの現在角度
	Vector3 playerRot = object_->GetWorldTransform().rotate_;

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

	//線形補間で回転
	object_->GetWorldTransform().rotate_ = Lerp(playerRot, rotate, rotStrength_ / 100.0f);
}

///=====================================================/// 
/// 飛行機状態の移動
///=====================================================///
void Player::JetMove() {

	//プレイヤーの現在角度
	Vector3 playerRot = object_->GetWorldTransform().rotate_;

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

	//線形補間で回転
	object_->GetWorldTransform().rotate_ = Lerp(playerRot, rotate, rotStrength_ / 100.0f);
}

///=====================================================/// 
/// 攻撃
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
/// 戦車状態の攻撃
///=====================================================///
void Player::TankAttack() {

	//タイマーが攻撃間隔を超えたら
	if (attackTimer_ >= tankAttackInterval_) {

		//スペースキーが押されていたら
		if (Input::GetInstance()->isPushKey(DIK_SPACE)) {

			//オブジェクトからレティクルへの方向
			Vector3 direction = reticle_->GetWorldPos() - object_->GetWorldTransform().GetWorldTranslate();

			//バレットマネージャーに弾を追加
			bulletManager_->AddBullet(
				object_->GetWorldTransform().translate_,
				Normalize(direction),
				BulletManager::BULLETTYPE::TANK
			);

			//攻撃のタイマーをリセット
			attackTimer_ = 0.0f;
		}
	}
}

///=====================================================/// 
/// 飛行機状態の攻撃
///=====================================================///
void Player::JetAttack() {

	//タイマーが攻撃間隔を超えたら
	if (attackTimer_ >= jetAttackInterval_) {

		//スペースキーが押されていたら
		if (Input::GetInstance()->isPushKey(DIK_SPACE)) {

			//オブジェクトからレティクルへの方向
			Vector3 direction = reticle_->GetWorldPos() - object_->GetWorldTransform().GetWorldTranslate();

			//バレットマネージャーに弾を追加
			bulletManager_->AddBullet(
				object_->GetWorldTransform().translate_,
				Normalize(direction),
				BulletManager::BULLETTYPE::JET
			);

			//攻撃のタイマーをリセット
			attackTimer_ = 0.0f;
		}
	}
}

///=====================================================/// 
/// 衝突時処理
///=====================================================///
void Player::IsCollision() {

}