#include "Object2D.h"

#include <Base/DirectXCommon.h>
#include <3d/Camera/Camera.h>
#include <Base/Renderer.h>
#include <Object/Base/ObjectCommonBase.h>
#include <2d/Sprite/SpriteManager.h>
#include <2d/Sprite/Sprite.h>

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

	transform_.translate_ = { translate_.x,translate_.y,0.0f };
	transform_.rotate_ = { 0.0f,0.0f,rotate_ };
	transform_.scale_ = { size_.x,size_.y,1.0f };

	transform_.UpdateMatrix();

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

	size_ = sprite_->GetSize();
}