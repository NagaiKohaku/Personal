#pragma once

#include <Base/DirectXCommon.h>
#include <Math/Vector/Vector4.h>
#include <Math/Matrix/Matrix4x4.h>

#include <d3d12.h>
#include <string>
#include <wrl.h>

namespace MyEngine {

	class ModelMaterial {

	public:

		struct Material {
			Vector4 color;
			int32_t enableLighting;
			float padding[3];
			Matrix4x4 uvTransform;
			float shininess;
			float environmentCoefficient;
		};

	public:

		void Initialize(DirectXCommon* directCommon);

		void Draw(DirectXCommon* directCommon);

	private:

		Material* materialData_;

		Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_;

	public:

		void SetColor(const Vector4& color) { materialData_->color = color; }

		Vector4 GetColor() { return materialData_->color; }

		void SetEnableLighting(const int32_t& flag) { materialData_->enableLighting = flag; }

		int32_t GetEnableLighting() { return materialData_->enableLighting; }

		void SetUVTransform(const Matrix4x4& transform) { materialData_->uvTransform = transform; }

		Matrix4x4 GetUVTransform() { return materialData_->uvTransform; }

		void SetShininess(const float& num) { materialData_->shininess = num; }

		float GetShininess() { return materialData_->shininess; }

		void SetEnvironmentCoefficient(const float& num) { materialData_->environmentCoefficient = num; }

		float GetEnvironmentCoefficient() { return materialData_->enableLighting; }

	};
}