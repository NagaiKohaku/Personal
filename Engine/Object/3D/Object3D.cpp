#include "Object3D.h"

#include <Object/3D/Object3DCommon.h>
#include <3d/Camera/Camera.h>
#include <Base/Renderer.h>
#include <3d/Model/ModelManager.h>
#include <2d/Sprite/TextureManager.h>
#include <Math/Utility/MakeMatrixMath.h>

using namespace MyEngine;

void Object3D::Initialize(ObjectCommonBase* objectCommonPtr, Camera* cameraPtr, Renderer* rendererPtr) {

	ObjectBase::Initialize(objectCommonPtr, cameraPtr, rendererPtr);

	CreatePartWVPResources(1);

	partTransforms_[0] = transform_;

	camera_ = dynamic_cast<Object3DCommon*>(objectCommon_)->GetDefaultCamera();
}

void Object3D::Update() {

	viewProjectionMatrix_ = camera_->Get3DViewProjectionMatrix();

	ObjectBase::Update();

	for (size_t i = 0; i < partTransforms_.size(); i++) {
		partTransforms_[i].UpdateMatrix();

		mappedPartMatrices_[i]->WVP = partTransforms_[i].GetWorldMatrix() * viewProjectionMatrix_;
		mappedPartMatrices_[i]->World = partTransforms_[i].GetWorldMatrix();
		mappedPartMatrices_[i]->WorldInverseTranspose = TransposeMatrix(Inverse4x4(partTransforms_[i].GetWorldMatrix()));
	}
}

void Object3D::Draw(LayerType layer) {

	std::function<void()> func;

	func = [this]() {

		//3Dオブジェクトの描画前処理
		objectCommon_->CommonDrawSetting();

		if (model_) {
			const auto& parts = model_->GetMeshParts();
			for (size_t i = 0; i < parts.size(); i++) {
				const auto& part = parts[i];
				auto mesh = model_->GetMesh(i);

				//座標変換行列データの設定 (パーツ単位のTransformationMatrix)
				objectCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, partTransformationMatrixResources_[i].Get()->GetGPUVirtualAddress());

				//メッシュデータの設定
				mesh->SendDataForGPU();

				//マテリアルデータの設定
				objectCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, part.materialResource.Get()->GetGPUVirtualAddress());

				//テクスチャデータの設定
				objectCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(part.textureFilePath));

				//描画コマンド発行
				objectCommon_->GetDxCommon()->GetCommandList()->DrawIndexedInstanced(UINT(mesh->GetIndexCount()), 1, 0, 0, 0);
			}
		}
		};

	//レンダラーに描画関数を登録
	renderer_->AddDraw(layer, true, func);
}

void Object3D::SetModel(const std::string& modelName) {

	model_ = std::move(ModelManager::GetInstance()->FindModel(modelName));

	if (model_) {

		size_t partCount = model_->GetMeshParts().size();

		CreatePartWVPResources(static_cast<int32_t>(partCount));
	}
}

void Object3D::SetObjectData(const ObjectData& objectData) {

	//モデルの設定
	SetModel(objectData.filename);

	transform_.translate_ = objectData.position;

	transform_.rotate_ = objectData.rotation;

	transform_.scale_ = objectData.scale;
}

void Object3D::CreatePartWVPResources(int32_t count) {

	partTransforms_.resize(count);
	partTransformationMatrixResources_.resize(count);
	mappedPartMatrices_.resize(count);

	for (size_t i = 0; i < count; i++) {
		partTransforms_[i].Initialize();

		if (i >= 1) {

			partTransforms_[i].SetParent(&partTransforms_[0]);
		}

		partTransformationMatrixResources_[i] = objectCommon_->GetDxCommon()->CreateBufferResource(sizeof(TransformationMatrix));

		partTransformationMatrixResources_[i]->Map(0, nullptr, reinterpret_cast<void**>(&mappedPartMatrices_[i]));
		mappedPartMatrices_[i]->WVP = MakeIdentity4x4();
		mappedPartMatrices_[i]->World = MakeIdentity4x4();
		mappedPartMatrices_[i]->WorldInverseTranspose = MakeIdentity4x4();
	}
}
