#include "Building.h"

#include "3d/Model/Model.h"
#include "3d/Model/ModelManager.h"

///=====================================================/// 
/// 建物オブジェクトを初期化
///=====================================================///
void Building::Initialize(Vector3 pos, Vector3 rotate) {

	ModelManager::GetInstance()->LoadModel("Building", "Building", "Building");

	//建物オブジェクト生成
	object_ = std::make_unique<Object3D>();

	object_->Initialize();

	object_->SetModel("Building");

	object_->GetWorldTransform().translate_ = pos;

	object_->GetWorldTransform().rotate_ = rotate;

	Matrix4x4 posMatrix = MakeTranslateMatrix(Vector3(0.0f, -pos.y, 0.0f));

	Matrix4x4 rotMatrix = MakeRotateMatrix(Vector3(0.0f, 0.0f, 0.0f));

	Matrix4x4 scaleMatrix = MakeScaleMatrix(Vector3(1.0f, 1.0f, 1.0f));

	Matrix4x4 transformMatrix = scaleMatrix * rotMatrix * posMatrix;

	object_->GetModel()->SetSubmeshLocalTransform(static_cast<size_t>(1), transformMatrix);

	object_->GetModel()->UpdateSubmeshTransformsCPU();

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
	object_->Draw(LayerType::Object);
}