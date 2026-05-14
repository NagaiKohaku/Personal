#include "Sprite.h"

#include <Base/WinApp.h>
#include <Base/DirectXCommon.h>

#include <Object/2D/Texture/TextureManager.h>

#include <Math/Utility/MakeMatrixMath.h>

#include <imgui.h>
#include <fstream>
#include <filesystem>

namespace MyEngine {

	///=====================================================/// 
	/// 初期化
	///=====================================================///
	void Sprite::Initialize(const std::string& fileName, Object2DCommon* object2DCommonPtr) {

		/// === インスタンスの取得 === ///

		//スプライト基底のインスタンスを取得
		object2DCommon_ = object2DCommonPtr;

		/// === マテリアルリソースの作成 === ///

		//リソースを作成
		materialResource_ = object2DCommon_->GetDxCommon()->CreateBufferResource(sizeof(Material));

		//書き込むためのアドレスを取得する
		materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

		//マテリアルデータの設定
		materialData_->uvTransform = MakeIdentity4x4();

		/// === テクスチャの読み込み === ///

		fileName_ = fileName;

		currentTextureIndex_ = 0;

		for (const auto& entry : std::filesystem::directory_iterator("Resource/Sprite/" + fileName_ + "/")) {
			if (entry.path().extension() == ".png") {

				TextureManager::GetInstance()->LoadTexture("Resource/Sprite/" + fileName_ + "/" + entry.path().filename().string());

				texturePaths_.push_back(entry.path().filename().string());
			}
		}

		//サイズをテクスチャの解像度に合わせる
		AdjustTextureSize();

		/// === その他変数の初期化 === ///

		//テクスチャの左上座標の初期化
		textureLeftTop_ = Vector2(0.0f, 0.0f);

	}

	///=====================================================/// 
	/// 更新
	///=====================================================///
	void Sprite::Update() {

		/// === アンカーポイントに基づいて四隅の頂点位置を計算 === ///

		//アンカーポイントから四点を計算
		float left = 0.0f - config_.anchorPoint.x;
		float right = 1.0f - config_.anchorPoint.x;
		float top = 0.0f - config_.anchorPoint.y;
		float bottom = 1.0f - config_.anchorPoint.y;

		/// === X軸・Y軸反転フラグを反映 === ///

		//X軸の反転
		if (config_.isFlipX) {
			left = -left;
			right = -right;
		}

		//Y軸の反転
		if (config_.isFlipY) {
			top = -top;
			bottom = -bottom;
		}

		/// === テクスチャUV座標を計算 === ///

		std::string fileName = "Resource/Sprite/" + fileName_ + "/" + texturePaths_[currentTextureIndex_];

		//テクスチャのメタデータを取得
		const DirectX::TexMetadata& metadata = TextureManager::GetInstance()->GetMetaData(fileName);

		//テクスチャの四点を計算
		float texLeft = textureLeftTop_.x / metadata.width;
		float texRight = (textureLeftTop_.x + textureSize_.x) / metadata.width;
		float texTop = textureLeftTop_.y / metadata.height;
		float texBottom = (textureLeftTop_.y + textureSize_.y) / metadata.height;

		materialData_->color          = config_.color;
		materialData_->enableLighting = config_.enableLighting;
		materialData_->enableEdit     = config_.enableEdit;
		materialData_->ratio          = config_.ratio;
		materialData_->brightness     = config_.brightness;
	}

	///=====================================================/// 
	/// 描画
	///=====================================================///
	void Sprite::Draw() {

		std::string fileName = "Resource/Sprite/" + fileName_ + "/" + texturePaths_[currentTextureIndex_];

		//頂点データの設定
		object2DCommon_->GetDxCommon()->GetCommandList()->IASetVertexBuffers(0, 1, &object2DCommon_->GetVertexBufferView());

		//頂点番号データの設定
		object2DCommon_->GetDxCommon()->GetCommandList()->IASetIndexBuffer(&object2DCommon_->GetIndexBufferView());

		//マテリアルデータの設定
		object2DCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());

		//テクスチャの設定
		object2DCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(fileName));

		//描画命令
		object2DCommon_->GetDxCommon()->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
	}

	///=====================================================/// 
	/// 現在のテクスチャを次のものに切り替え
	///=====================================================///
	void Sprite::NextTexture() {

		//テクスチャ番号をインクリメント
		currentTextureIndex_++;

		//範囲外なら0に戻す
		if (currentTextureIndex_ >= static_cast<int>(texturePaths_.size())) {
			currentTextureIndex_ = 0;
		}
	}

	///=====================================================/// 
	/// ImGuiの表示
	///=====================================================///
	void Sprite::DisplayImGui() {

#ifdef _USE_IMGUI

		ImGui::ColorEdit4("Color", &materialData_->color.x);
		ImGui::DragFloat2("AnchorPoint", &config_.anchorPoint.x, 0.01f, 0.0f, 1.0f);
		ImGui::Checkbox("IsFlipX", &config_.isFlipX);
		ImGui::Checkbox("IsFlipY", &config_.isFlipY);
		ImGui::DragFloat2("TexLeftTop", &textureLeftTop_.x, 0.1f);
		ImGui::DragFloat2("TexSize", &textureSize_.x, 0.1f);

#endif // _USE_IMGUI

	}

	///=====================================================/// 
	/// スプライトのサイズをテクスチャのサイズに合わせる
	///=====================================================///
	void Sprite::AdjustTextureSize() {

		std::string fileName = "Resource/Sprite/" + fileName_ + "/" + texturePaths_[currentTextureIndex_];

		//テクスチャのメタデータを取得
		const DirectX::TexMetadata& metadata = TextureManager::GetInstance()->GetMetaData(fileName);

		//テクスチャのサイズを取得
		textureSize_.x = static_cast<float>(metadata.width);
		textureSize_.y = static_cast<float>(metadata.height);
	}

	Sprite& Sprite::SetColor(const Vector4& color) {
		config_.color = color;
		return *this;
	}

	Sprite& Sprite::SetAnchorPoint(const Vector2& anchorPoint) {
		config_.anchorPoint = anchorPoint;
		return *this;
	}

	Sprite& Sprite::SetRatio(const float& ratio) {
		config_.ratio = ratio;
		return *this;
	}

	Sprite& Sprite::SetBrightness(const float& brightness) {
		config_.brightness = brightness;
		return *this;
	}

	Sprite& Sprite::SetEnableLighting(bool enableLighting) {
		config_.enableLighting = enableLighting;
		return *this;
	}

	Sprite& Sprite::SetEnableEdit(bool enableEdit) {
		config_.enableEdit = enableEdit;
		return *this;
	}

	Sprite& Sprite::SetIsFlipX(bool isFlipX) {
		config_.isFlipX = isFlipX;
		return *this;
	}

	Sprite& Sprite::SetIsFlipY(bool isFlipY) {
		config_.isFlipY = isFlipY;
		return *this;
	}
}