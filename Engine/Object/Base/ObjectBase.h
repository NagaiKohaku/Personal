#pragma once

#include <Base/LayerType.h>

#include <Math/Transform/WorldTransform.h>
#include <Math/Matrix/TransformationMatrix.h>

#include <d3d12.h>
#include <wrl.h>

namespace MyEngine {

	class ObjectCommonBase;
	class Renderer;
	class Camera;

	class ObjectBase {

	public:

		virtual void Initialize(ObjectCommonBase* objectCommonPtr, Camera* cameraPtr, Renderer* rendererPtr);

		virtual void Update();

		virtual void Draw(LayerType layer) = 0;

	protected:

		Camera* camera_;

		Renderer* renderer_;

		ObjectCommonBase* objectCommon_;

		WorldTransform transform_;

		Matrix4x4 viewProjectionMatrix_;

		//座標変換行列リソース
		Microsoft::WRL::ComPtr<ID3D12Resource> WVPResource_ = nullptr;

		//座標変換行列データ
		TransformationMatrix* WVPData_ = nullptr;

		bool isUpdateTransform_ = true;

	public:

		WorldTransform& GetWorldTransform() { return transform_; }

		bool GetIsUpdateTransform() const { return isUpdateTransform_; }

		void SetIsUpdateTransform(const bool& flag) { isUpdateTransform_ = flag; }

	};
}