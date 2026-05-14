#include "ModelMaterial.h"

#include <Math/Utility/MakeMatrixMath.h>

using namespace MyEngine;

void ModelMaterial::Initialize(DirectXCommon* directCommon) {

	materialResource_ = directCommon->CreateBufferResource(sizeof(Material));

	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

	materialData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialData_->enableLighting = true;
	materialData_->uvTransform = MakeIdentity4x4();
	materialData_->shininess = 50.0f;
	materialData_->environmentCoefficient = 1.0f;
}

void ModelMaterial::Draw(DirectXCommon* directCommon) {

	directCommon->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_.Get()->GetGPUVirtualAddress());
}