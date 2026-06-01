#pragma once

#include <Object/Base/ObjectTag.h>
#include <Object/Component/Base/ComponentBase.h>
#include <Object/Component/WorldTransform/WorldTransform.h>

#include <string>
#include <memory>
#include <vector>

namespace MyEngine {

	class GameObject {

	public:

		virtual ~GameObject() = default;

		virtual void Initialize();

		virtual void Update();

		virtual void Draw();

	private:

		std::string name_;

		ObjectTag tag_;

		std::vector<std::unique_ptr<WorldTransform>> partTransforms_;

		std::vector<std::shared_ptr<ComponentBase>> components_;

		bool isDraw_;

		bool isActive_;

	public:

		void SetName(std::string name) { name_ = std::move(name); }

		const std::string& GetName() const { return name_; }

		void SetObjectTag(ObjectTag tag) { tag_ = tag; }

		ObjectTag GetObjectTag() const { return tag_; }

		void SetComponent(std::shared_ptr<ComponentBase> component) { components_.push_back(component); }
	};

}