#include "Ground.h"

#include "3d/Object/Object3D.h"
#include "2d/Sprite/TextureManager.h"
#include "3d/Model/Model.h"

#include "Math/MakeMatrixMath.h"

void Ground::Initialize() {

	// オブジェクトの生成
	object_ = std::make_unique<Object3D>();

	// オブジェクトの初期化
	object_->Initialize();

	object_->SetModel("Plane");

	TextureManager::GetInstance()->LoadTexture("Resource/Texture/RunWay.png");

	object_->GetModel()->SetTextureFilePath("Resource/Texture/RunWay.png");

	// オブジェクトのスケール設定
	object_->GetWorldTransform().scale_ = { 200.0f, 1000.0f, 1.0f };

	object_->GetWorldTransform().rotate_ = { 3.14f / 2.0f,0.0f,0.0f };

	object_->GetModel()->SetEnvironmentCoefficient(0.0f);

	uvTimer_ = 0.0f;
}

void Ground::Update() {

	// オブジェクトの更新
	object_->Update();

	uvTimer_ += 1.0f / 60.0f;

	Vector3 uvTranslate = { 0.0f,uvTimer_,0.0f };
	Matrix4x4 uvTransformMatrix = MakeTranslateMatrix(uvTranslate);
	Matrix4x4 uvScaleMatrix = MakeIdentity4x4();
	Matrix4x4 uvRotateMatrix = MakeIdentity4x4();

	object_->GetModel()->SetUVTransform((uvScaleMatrix * uvRotateMatrix) * uvTransformMatrix);
}

void Ground::Draw() {

	// オブジェクトの描画
	object_->Draw(LayerType::Object);
}