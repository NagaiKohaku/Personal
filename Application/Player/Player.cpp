#include "Player.h"

#include "Base/Input.h"

void Player::Initialize() {
	//プレイヤーの生成
	player_ = std::make_unique<Object3D>();
	//座標の設定
	player_->GetWorldTransform().translate_ = { 0.0f,1.0f,0.0f };
	//モデルの設定
	player_->SetModel("Cube");
}

void Player::Update() {

	Move();

	//プレイヤーの更新
	player_->Update();

}

void Player::Draw() {

	//プレイヤーの描画
	player_->Draw(LayerType::Object);
}

void Player::Move() {

	if (Input::GetInstance()->isPushKey(DIK_W)) {

		player_->GetWorldTransform().translate_.y += 0.1f;
	}

	if (Input::GetInstance()->isPushKey(DIK_S)) {

		player_->GetWorldTransform().translate_.y -= 0.1f;
	}

	if (Input::GetInstance()->isPushKey(DIK_A)) {

		player_->GetWorldTransform().translate_.x -= 0.1f;
	}

	if (Input::GetInstance()->isPushKey(DIK_D)) {

		player_->GetWorldTransform().translate_.x += 0.1f;
	}
}
