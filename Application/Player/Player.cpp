#include "Player.h"

#include "Bullet/BulletManager.h"

#include "Base/Input.h"

void Player::Initialize() {

	//プレイヤーの生成
	player_ = std::make_unique<Object3D>();

	//座標の設定
	player_->GetWorldTransform().translate_ = { 0.0f,1.0f,0.0f };

	//モデルの設定
	player_->SetModel("Cube");

	//移動速度の設定
	moveSpeed_ = 0.1f;

	//攻撃の間隔
	attackInterval_ = 0.1f;

	//攻撃のタイマー
	attackTimer_ = 0.0f;
}

void Player::Update() {

	//移動
	Move();

	//攻撃
	Attack();

	//プレイヤーの更新
	player_->Update();

}

void Player::Draw() {

	//プレイヤーの描画
	player_->Draw(LayerType::Object);
}

void Player::Move() {

	if (Input::GetInstance()->isPushKey(DIK_W)) {

		player_->GetWorldTransform().translate_.y += moveSpeed_;
	}

	if (Input::GetInstance()->isPushKey(DIK_S)) {

		player_->GetWorldTransform().translate_.y -= moveSpeed_;
	}

	if (Input::GetInstance()->isPushKey(DIK_A)) {

		player_->GetWorldTransform().translate_.x -= moveSpeed_;
	}

	if (Input::GetInstance()->isPushKey(DIK_D)) {

		player_->GetWorldTransform().translate_.x += moveSpeed_;
	}
}

void Player::Attack() {

	attackTimer_ += 1.0f / 60.0f;

	//攻撃の間隔を超えたら
	if (attackTimer_ >= attackInterval_) {

		if (Input::GetInstance()->isPushKey(DIK_SPACE)) {

			//弾を追加
			bulletManager_->AddBullet(player_->GetWorldTransform().translate_);

			//攻撃のタイマーをリセット
			attackTimer_ = 0.0f;
		}
	}

}