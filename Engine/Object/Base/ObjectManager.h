#pragma once

#include <Base/DirectXCommon.h>
#include <Object/Base/GameObject.h>

#include <map>
#include <string>

namespace MyEngine {
	class ObjectManager {

	public:

		void Initialize(DirectXCommon* directCommonPtr);

		void Update();

		void Draw();

		void CreateGameObject(const std::string& name);

	private:

		DirectXCommon* directCommon_;

		std::map<std::string, std::unique_ptr<GameObject>> gameObjects_;

	public:

		GameObject* GetGameObject(const std::string& name);

	};
}