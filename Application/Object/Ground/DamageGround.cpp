#include "DamageGround.h"

using namespace MyEngine;

DamageGround::~DamageGround() {

	collider_->Remove();
}

void DamageGround::Initialize(Vector3 pos) {

	object_ = std::make_unique<Object3D>();

	object_->Initialize();

	object_->SetModel("Cube");

	object_->GetWorldTransform().scale_ = { 7.0f,0.1f,10.0f };

	Vector3 scale = object_->GetWorldTransform().scale_;

	object_->GetWorldTransform().translate_ = { pos.x,pos.y + scale.y / 2.0f,pos.z };

	object_->GetModel()->SetColor({ 1.0f,0.0f,0.0f,1.0f });

	//生成
	collider_ = std::make_unique<AABBCollider>();

	//初期化
	collider_->Initialize(&object_->GetWorldTransform());

	//タグの設定
	collider_->SetTag(Collider::Tag::ENEMYBULLET);

	//大きさの設定
	collider_->SetTransform(Vector3(0.0f, 0.0f, 0.0f), Vector3(scale.x / 2.0f, 1.0f, scale.z / 2.0f));

	speed_ = 200.0f;

	deadLine_ = -100.0f;

	isDead_ = false;

	object_->Update();
}

void DamageGround::Update() {

	if (object_->GetWorldTransform().translate_.z <= deadLine_) {
		isDead_ = true;
	}

	object_->GetWorldTransform().translate_.z -= speed_ * 1.0f / 60.0f;

	object_->Update();

}

void DamageGround::TransformUpdate() {

	// オブジェクトの座標のみ更新
	object_->Update();

}

void DamageGround::Draw() {

	object_->Draw(LayerType::OBJECT);
}