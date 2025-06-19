#include "PlayerBullet.h"

PlayerBullet::~PlayerBullet() {

	collider_->Remove();
}

void PlayerBullet::Initialize(Vector3 pos) {

	//弾の生成
	bullet_ = std::make_unique<Object3D>();

	//弾の初期化
	bullet_->Initialize();

	//座標の設定
	bullet_->GetWorldTransform().translate_ = pos;

	//サイズの設定
	bullet_->GetWorldTransform().scale_ = { 0.5f,0.5f,0.5f };

	//モデルの設定
	bullet_->SetModel("Sphere");

	//弾のコライダーを生成
	collider_ = std::make_unique<SphereCollider>();

	//弾のコライダーの初期化
	collider_->Initialize(&bullet_->GetWorldTransform());

	collider_->SetTag(Collider::Tag::PLAYERBULLET);

	collider_->SetRadius(0.5f);

	//弾の移動速度
	speed_ = 0.5f;

	//弾の最大寿命
	lifeTimeMax_ = 2.0f;
}

void PlayerBullet::Update() {

	//弾の寿命を更新
	lifeTime_ += 1.0f / 60.0f;

	//弾の寿命が最大寿命を超えたら
	if (lifeTime_ >= lifeTimeMax_) {

		isDead_ = true;
	}

	//弾の移動
	Move();

	//弾の更新
	bullet_->Update();

	//弾のコライダーの更新
	collider_->Update();
}

void PlayerBullet::Draw() {

	//弾の描画
	bullet_->Draw(LayerType::Object);

	//弾のコライダーの描画
	collider_->Draw();
}

void PlayerBullet::Move() {

	velocity_ = { 0.0f,0.0f,speed_ };

	//弾の移動
	bullet_->GetWorldTransform().translate_ += velocity_;
}
