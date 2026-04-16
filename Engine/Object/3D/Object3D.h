#pragma once

#include <Object/Base/ObjectBase.h>
#include <Object/Model/Model.h>
#include <LevelEditor/ObjectData.h>
#include <Base/Renderer.h>
#include <Math/Matrix/TransformationMatrix.h>

#include <string>
#include <memory>

namespace MyEngine {

	class Object3D : public ObjectBase {

	public:

		void Initialize(ObjectCommonBase* objectCommonPtr, Camera* cameraPtr, Renderer* rendererPtr) override;

		void Update() override;

		void Draw(LayerType layer) override;

	private:

		//モデル
		Model* model_;

		//パーツごとのトランスフォーム
		std::vector<WorldTransform> partTransforms_;

		//パーツごとのTransformationMatrixリソース
		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> partTransformationMatrixResources_;

		//パーツごとのマップ済みポインタ
		std::vector<TransformationMatrix*> mappedPartMatrices_;

	public:

		/// <summary>
		/// ワールドトランスフォームを取得
		/// </summary>
		/// <returns>ワールドトランスフォーム</returns>
		WorldTransform& GetWorldTransform() { return partTransforms_[0]; }

		/// <summary>
		/// モデルを取得
		/// </summary>
		/// <returns>モデル</returns>
		Model* GetModel() const { return model_; }

		/// <summary>
		/// パーツのワールドトランスフォームを取得
		/// </summary>
		/// <param name="index">パーツのインデックス番号</param>
		/// <returns>指定パーツのワールドトランスフォーム</returns>
		WorldTransform& GetPartTransform(int32_t index) { return partTransforms_[index]; }

		/// <summary>
		/// パーツのワールドトランスフォームを設定
		/// </summary>
		/// <param name="index">パーツのインデックス番号</param>
		/// <param name="transform">設定するトランスフォーム</param>
		void SetPartTransform(int32_t index, const WorldTransform& transform) { partTransforms_[index] = transform; }

		void SetModel(const std::string& modelName);

		void SetObjectData(const ObjectData& objectData);

	private:

		// パーツごとのWVPリソースを作成する
		void CreatePartWVPResources(int32_t count);

	};
}