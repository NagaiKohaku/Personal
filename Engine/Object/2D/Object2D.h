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

	public:

		Sprite* GetSprite() const { return sprite_.get(); }

		Vector2 GetTranslate() const { return { transform_.translate_.x, transform_.translate_.y }; }

		Vector2 GetSize() const { return { transform_.scale_.x, transform_.scale_.y }; }

		float GetRotate() const { return transform_.rotate_.z; }

		void SetSprite(const std::string& spriteName);

		void SetTranslate(const Vector2& translate) { transform_.translate_ = { translate.x, translate.y, 0.0f }; }

		void SetSize(const Vector2& size) { transform_.scale_ = { size.x, size.y, 1.0f }; }

		void SetRotate(const float& rotate) { transform_.rotate_ = { 0.0f, 0.0f, rotate }; }

	};
}
