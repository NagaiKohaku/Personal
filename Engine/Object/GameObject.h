#pragma once

#include <Object/ObjectTag.h>

#include <string>
#include <memory>

namespace MyEngine {

	class GameObject {

	public:

		void Initialize();

		void Update();

		void Draw();

	private:

		std::string name_;

		ObjectTag tag_;

		bool isDraw_;

		bool isActive_;

	};

}