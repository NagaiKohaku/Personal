#include "Object2D.h"

#include <Base/DirectXCommon.h>
#include <3d/Camera/Camera.h>
#include <Base/Renderer.h>
#include <Object/Base/ObjectCommonBase.h>
#include <2d/Sprite/SpriteManager.h>
#include <2d/Sprite/Sprite.h>
#include <Math/Utility/MakeMatrixMath.h>

using namespace MyEngine;

void Object2D::Initialize(ObjectCommonBase* objectCommonPtr, Camera* cameraPtr, Renderer* rendererPtr) {

	ObjectBase::Initialize(objectCommonPtr, cameraPtr, rendererPtr);

	viewProjectionMatrix_ = camera_->Get2DViewProjectionMatrix();

	isUpdateTransform_ = false;
}

void Object2D::Update() {

	if (sprite_) {
		sprite_->Update();
	}

	// 2D専用の行列計算（アンカーポイント対応版）
	Matrix4x4 mOffset = MakeTranslateMatrix({ -sprite_->GetConfig().anchorPoint.x, -sprite_->GetConfig().anchorPoint.y, 0.0f });
	Matrix4x4 mScale = MakeScaleMatrix(transform_.scale_);
	Matrix4x4 mRotate = MakeRotateZMatrix(transform_.rotate_.z);
	Matrix4x4 mTranslate = MakeTranslateMatrix(transform_.translate_);

	// 合成 (Offset -> Scale -> Rotate -> Translate)
	transform_.SetLocalMatrix(mOffset * mScale * mRotate * mTranslate);

	transform_.UpdateWorldMatrixOnly();

	ObjectBase::Update();
}

void Object2D::Draw(LayerType layer) {

	std::function<void()> func;

	func = [this]() {

		//2Dオブジェクトの描画前処理
		objectCommon_->CommonDrawSetting();

		//座標変換行列データの設定
		objectCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, WVPResource_.Get()->GetGPUVirtualAddress());

		//スプライトが割り当てられていれば描画する
		if (sprite_) {
			sprite_->Draw();
		}
		};

	//レンダラーに描画関数を登録
	renderer_->AddDraw(layer, true, func);
}

void Object2D::SetSprite(const std::string& spriteName) {

	sprite_ = SpriteManager::GetInstance()->FindSprite(spriteName);

	transform_.scale_ = { sprite_->GetTextureSize().x,sprite_->GetTextureSize().y,1.0f };
}