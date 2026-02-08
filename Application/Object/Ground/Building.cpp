#include "Building.h"

#include "3d/Model/Model.h"

using namespace MyEngine;

///=====================================================/// 
/// 建物オブジェクトを初期化
///=====================================================///
void Building::Initialize(Vector3 pos, Vector3 scale) {

	//建物オブジェクト生成
	object_ = std::make_unique<Object3D>();

	object_->Initialize();

	object_->SetModel("Cube");

	object_->GetWorldTransform().scale_ = scale;

	object_->GetWorldTransform().translate_ = pos;

	//移動速度設定
	speed_ = 200.0f;
}

///=====================================================/// 
/// 建物オブジェクトの更新処理
///=====================================================///
void Building::Update() {

	//建物更新
	object_->Update();

	//Z方向に移動
	object_->GetWorldTransform().translate_.z -= speed_ * 1.0f / 60.0f;
}

///=====================================================/// 
/// 建物オブジェクトの座標更新
///=====================================================///
void Building::TransformUpdate() {

	//建物座標更新
	object_->Update();
}

///=====================================================/// 
/// 建物オブジェクトを描画
///=====================================================///
void Building::Draw() {

	//建物描画
	object_->Draw(LayerType::OBJECT);
}