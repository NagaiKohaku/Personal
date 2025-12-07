#include "Boss.h"

#include "3d/Model/ModelManager.h"



void Boss::Initialize() {

	ModelManager::GetInstance()->LoadModel("Boss", "Boss", "Boss");

	//ボスオブジェクト生成
	object_ = std::make_unique<Object3D>();
	object_->Initialize();
	object_->SetModel("Boss");
	object_->GetWorldTransform().translate_ = { 0.0f, 30.0f, 200.0f };
	object_->GetWorldTransform().rotate_ = { 0.25f,3.14f,0.0f };

	barrier01Transform_.Initialize();
	barrier02Transform_.Initialize();
	barrier03Transform_.Initialize();
	barrier04Transform_.Initialize();
}

void Boss::Update() {

	float pi = 3.14f;

	//ボスオブジェクト更新
	object_->Update();

	barrier01Transform_.UpdateMatrix();
	barrier02Transform_.UpdateMatrix();
	barrier03Transform_.UpdateMatrix();
	barrier04Transform_.UpdateMatrix();

	object_->GetModel()->SetSubmeshLocalTransform(static_cast<size_t>(1), barrier01Transform_.GetWorldMatrix());
	object_->GetModel()->SetSubmeshLocalTransform(static_cast<size_t>(2), barrier02Transform_.GetWorldMatrix());
	object_->GetModel()->SetSubmeshLocalTransform(static_cast<size_t>(3), barrier03Transform_.GetWorldMatrix());
	object_->GetModel()->SetSubmeshLocalTransform(static_cast<size_t>(4), barrier04Transform_.GetWorldMatrix());

	object_->GetModel()->UpdateSubmeshTransformsCPU();
}

void Boss::Draw() {

	//ボスオブジェクト描画
	object_->Draw(LayerType::Object);
}