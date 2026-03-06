#pragma once

#include <Object/Base/ObjectBase.h>
#include <3d/Model/Model.h>
#include <LevelEditor/ObjectData.h>

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
		std::unique_ptr<Model> model_;

	public:

		/// <summary>
		/// ワールドトランスフォームを取得
		/// </summary>
		/// <returns>ワールドトランスフォーム</returns>
		WorldTransform& GetWorldTransform() { return transform_; }

		/// <summary>
		/// モデルを取得
		/// </summary>
		/// <returns>モデル</returns>
		Model* GetModel() const { return model_.get(); }

		void SetModel(const std::string& modelName);

		void SetObjectData(const ObjectData& objectData);

	};
}