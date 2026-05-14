#pragma once

#include <Object/Base/GameObject.h>

#include <vector>
#include <string>

namespace MyEngine {
	class ObjectManager {

	public:

		void Initialize();

		void Update();

		void Draw();

		void CreateGameObject();

	private:

		std::vector<std::unique_ptr<GameObject>> gameObjects_;

	public:

		GameObject* GetGameObject(std::string name);

	};
}