#pragma once

#include <Base/DirectXCommon.h>
#include <Object/Component/Base/ComponentBase.h>
#include <Object/Component/Geometry/Model/Model.h>
#include <Object/Component/Geometry/Sprite/Sprite.h>
#include <Object/Component/WorldTransform/WorldTransform.h>

#include <memory>
#include <string>

namespace MyEngine {

	class ComponentManager {

	public:

		static ComponentManager* GetInstance();

		void Initialize(DirectXCommon* directCommonPtr);

		std::shared_ptr<Model> CreateModelComponent(const std::string& fileName);

		std::shared_ptr<Sprite> CreateSpriteComponent();

		std::shared_ptr<WorldTransform> CreateWorldTransformComponent();

	private:

		DirectXCommon* directCommon_;

	};
}