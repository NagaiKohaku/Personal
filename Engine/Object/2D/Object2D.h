#pragma once

#include <Object/Base/ObjectBase.h>
#include <2D/Sprite/Sprite.h>

#include <Math/Vector/Vector2.h>

#include <string>
#include <memory>

namespace MyEngine {

	class Object2D : public ObjectBase {

	public:

		void Initialize(ObjectCommonBase* objectCommonPtr, Camera* cameraPtr, Renderer* rendererPtr) override;

		void Update() override;

		void Draw(LayerType layer) override;

	private:

		std::unique_ptr<Sprite> sprite_;

		Vector2 translate_;

		Vector2 size_;

		float rotate_;

	public:

		WorldTransform& GetWorldTransform() { return transform_; }

		Vector2 GetTranslate() const { return translate_; }

		Vector2 GetSize() const { return size_; }

		float GetRotate() const { return rotate_; }

		Sprite* GetSprite() const { return sprite_.get(); }

		void SetTranslate(const Vector2& translate) { translate_ = translate; }

		void SetSize(const Vector2& size) { size_ = size; }

		void SetRotate(const float& rotate) { rotate_ = rotate; }

		void SetSprite(const std::string& spriteName);

	};
}
