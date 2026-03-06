#include "ObjectBase.h"

#include <Base/DirectXCommon.h>
#include <Object/Base/ObjectCommonBase.h>
#include <3d/Camera/Camera.h>
#include <Math/Utility/MakeMatrixMath.h>

using namespace MyEngine;

void ObjectBase::Initialize(ObjectCommonBase* objectCommonPtr, Camera* cameraPtr, Renderer* rendererPtr) {

	/// === インスタンスの取得 === ///

	camera_ = cameraPtr;

	renderer_ = rendererPtr;

	//2Dオブジェクト基底のインスタンスを取得
	objectCommon_ = objectCommonPtr;

	/// === 座標変換行列リソースを作成 === ///

	//リソースの作成
	WVPResource_ = objectCommonPtr->GetDxCommon()->CreateBufferResource(sizeof(TransformationMatrix));

	//書き込むためのアドレスを取得する
	WVPResource_->Map(0, nullptr, reinterpret_cast<void**>(&WVPData_));

	//データを初期化
	WVPData_->WVP = MakeIdentity4x4();
	WVPData_->World = MakeIdentity4x4();
	WVPData_->WorldInverseTranspose = MakeIdentity4x4();

	transform_.Initialize();
}

void ObjectBase::Update() {

	if (isUpdateTransform_) {

		//トランスフォームの更新
		transform_.UpdateMatrix();
	}

	//ワールドビュープロジェクション行列
	Matrix4x4 worldViewProjectionMatrix = transform_.GetWorldMatrix();

	if (camera_) {

		/// === カメラ情報があったら === ///

		//ワールドビュープロジェクション行列の計算
		worldViewProjectionMatrix *= viewProjectionMatrix_;
	}

	//座標変換行列データの設定
	WVPData_->WVP = worldViewProjectionMatrix;
	WVPData_->World = transform_.GetWorldMatrix();
	WVPData_->WorldInverseTranspose = Inverse4x4(transform_.GetWorldMatrix());
}