#include "Enemy.h"

void Enemy::Initialize() {

	//敵の生成
	enemy_ = std::make_unique<Object3D>();

	//敵の初期化
	enemy_->Initialize();

	//座標の設定
	enemy_->GetWorldTransform().translate_ = { 0.0f,1.0f,0.0f };

	//モデルの設定
	enemy_->SetModel("Cube");

	collider_ = std::make_unique<SphereCollider>();

	collider_->Initialize(&enemy_->GetWorldTransform());

	collider_->SetTag(Collider::Tag::ENEMY);

	collider_->SetRadius(1.5f);

	explosiveEmitter_ = std::make_unique<EmitterGroup>();

	explosiveEmitter_->Initialize(camera_);

	explosiveEmitter_->LoadEmitter("BlockExplosive");

	animTimer_ = 0.0f;

	animMaxTimer_ = 5.0f;
}

void Enemy::Update() {

	//衝突判定
	IsCollision();

	if (isDead_) {
		Dead();
	}

	//敵の更新
	enemy_->Update();

	collider_->Update();

	explosiveEmitter_->GetWorldTransform().translate_ = enemy_->GetWorldTransform().translate_;

	explosiveEmitter_->Update();
}

void Enemy::Draw() {

	//敵の描画
	enemy_->Draw(LayerType::Object);

	collider_->Draw();

	explosiveEmitter_->Draw();
}

void Enemy::IsCollision() {

	if (collider_->GetHitTag() == Collider::Tag::PLAYERBULLET) {

		isDead_ = true;

		explosiveEmitter_->Emit();

		collider_->Remove();
	}
}

void Enemy::Dead() {

	animTimer_ += 1.0f / 60.0f;

	enemy_->GetWorldTransform().rotate_ = { -1.0f,0.0f,0.0f };

	enemy_->GetWorldTransform().translate_.y -= 0.1f;

	if (animTimer_ >= animMaxTimer_) {

		canRemove_ = true;
	}
}