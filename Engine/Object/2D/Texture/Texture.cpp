#include "Texture.h"

#include <Object/2D/Texture/TextureManager.h>

using namespace MyEngine;

void Texture::Initialize(const std::string& filePath = "") {

	if (filePath.empty()) {

		filePath_ = kDefaultTexterPath_;
	} else {

		filePath_ = filePath;
	}

	index_ = TextureManager::GetInstance()->GetSrvIndex(filePath_);

}

void Texture::Draw(DirectXCommon* directCommon) {

	directCommon->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(filePath_));
}

void Texture::SetFilePath(const std::string& filePath) {

	filePath_ = filePath;

	index_ = TextureManager::GetInstance()->GetSrvIndex(filePath);
}