#pragma once

#include <Base/DirectXCommon.h>

#include <string>

namespace MyEngine {

	class Texture {

	public:

		void Initialize(const std::string& filePath = "");

		void Draw(DirectXCommon* directCommon);

	private:

		const std::string kDefaultTexterPath_ = "Resource/Texture/white_128x128.png";

		std::string filePath_;

		uint32_t index_;

	public:

		void SetFilePath(const std::string& filePath);

	};
}