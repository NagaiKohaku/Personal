#include "Enemy.h"

void Enemy::Initialize() {

	//敵の生成
	enemy_ = std::make_unique<Object3D>();

	//敵の初期化
	enemy_->Initialize();

	//座標の設定
	enemy_->GetWorldTransform().translate_ = { 1.0f,1.0f,0.0f };

	//モデルの設定
	enemy_->SetModel("Cube");

	collider_ = std::make_unique<SphereCollider>();

	collider_->Initialize(&enemy_->GetWorldTransform());

	collider_->SetTag(Collider::Tag::ENEMY);

	collider_->SetRadius(1.5f);
}

void Enemy::Update() {

	//衝突判定
	IsCollision();

	//敵の更新
	enemy_->Update();

	collider_->Update();

}

void Enemy::Draw() {

	//敵の描画
	enemy_->Draw(LayerType::Object);

	collider_->Draw();
}

void Enemy::IsCollision() {

	if (collider_->GetHitTag() == Collider::Tag::PLAYER) {

		enemy_->GetModel()->SetColor({ 0.0f,1.0f,0.0f,1.0f });
	} else {

		enemy_->GetModel()->SetColor({ 1.0f,1.0f,1.0f,1.0f });
	}
}