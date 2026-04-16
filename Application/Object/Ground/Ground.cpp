#include "Ground.h"

#include <Object/3D/Object3D.h>
#include <2d/Sprite/TextureManager.h>
#include <Object/Model/Model.h>

#include <Math/Utility/MakeMatrixMath.h>

using namespace MyEngine;

void Ground::Initialize(EngineContext context) {

	// オブジェクトの生成
	object_ = context.objectManager->CreateObject3D();

	object_->SetModel("Plane");

	// テクスチャの読み込み
	TextureManager::GetInstance()->LoadTexture("Resource/Texture/RunWay.png");

	// モデルにテクスチャを設定
	object_->GetModel()->SetTextureFilePath("Resource/Texture/RunWay.png");

	// オブジェクトのスケール設定
	object_->GetWorldTransform().scale_ = { 200.0f, 1000.0f, 1.0f };

	object_->GetWorldTransform().rotate_ = { 3.14f / 2.0f,0.0f,0.0f };

	// 環境光係数の設定
	object_->GetModel()->GetModelParts()[0].material->environmentCoefficient = 0.0f;

	// UV座標Y初期化
	uvPosY = 0.0f;
}

void Ground::Update() {

	// オブジェクトの更新
	object_->Update();

	// UV座標Y更新
	uvPosY += 1.0f / 60.0f;

	// UV変換行列計算
	Vector3 uvTranslate = { 0.0f,uvPosY,0.0f };
	Matrix4x4 uvTransformMatrix = MakeTranslateMatrix(uvTranslate);
	Matrix4x4 uvScaleMatrix = MakeIdentity4x4();
	Matrix4x4 uvRotateMatrix = MakeIdentity4x4();

	// UV変換行列設定
	object_->GetModel()->GetModelParts()[0].material->uvTransform = (uvScaleMatrix * uvRotateMatrix) * uvTransformMatrix;
}

void Ground::TransformUpdate() {

	// オブジェクトの座標のみ更新
	object_->Update();
}

void Ground::Draw() {

	// オブジェクトの描画
	object_->Draw(LayerType::OBJECT);
}