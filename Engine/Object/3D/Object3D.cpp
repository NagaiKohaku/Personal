#include "Object3D.h"

#include <Object/3D/Object3DCommon.h>
#include <3d/Camera/Camera.h>
#include <Base/Renderer.h>
#include <3d/Model/ModelManager.h>

using namespace MyEngine;

void Object3D::Initialize(ObjectCommonBase* objectCommonPtr, Camera* cameraPtr, Renderer* rendererPtr) {

	ObjectBase::Initialize(objectCommonPtr, cameraPtr, rendererPtr);

	model_ = ModelManager::GetInstance()->FindModel("Default");

	camera_ = dynamic_cast<Object3DCommon*>(objectCommon_)->GetDefaultCamera();
}

void Object3D::Update() {

	viewProjectionMatrix_ = camera_->Get3DViewProjectionMatrix();

	ObjectBase::Update();
}

void Object3D::Draw(LayerType layer) {

	std::function<void()> func;

	func = [this]() {

		//3Dオブジェクトの描画前処理
		objectCommon_->CommonDrawSetting();

		//座標変換行列データの設定
		objectCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, WVPResource_.Get()->GetGPUVirtualAddress());

		//スプライトが割り当てられていれば描画する
		if (model_) {
			model_->Draw();
		}
		};

	//レンダラーに描画関数を登録
	renderer_->AddDraw(layer, true, func);
}

void Object3D::SetModel(const std::string& modelName) {

	model_ = ModelManager::GetInstance()->FindModel(modelName);
}

void Object3D::SetObjectData(const ObjectData& objectData) {

	//モデルの設定
	model_ = ModelManager::GetInstance()->FindModel(objectData.filename);

	transform_.translate_ = objectData.position;

	transform_.rotate_ = objectData.rotation;

	transform_.scale_ = objectData.scale;
}