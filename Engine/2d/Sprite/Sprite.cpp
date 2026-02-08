#include "Sprite.h"

#include "Base/WinApp.h"
#include "Base/DirectXCommon.h"

#include "2d/Sprite/TextureManager.h"

#include <Math/Utility/MakeMatrixMath.h>

#include "imgui.h"

#include "fstream"
#include "filesystem"

namespace MyEngine {

	///=====================================================/// 
	/// 初期化
	///=====================================================///
	void Sprite::Initialize(const std::string& fileName) {

		/// === インスタンスの取得 === ///

		//スプライト基底のインスタンスを取得
		spriteCommon_ = SpriteCommon::GetInstance();

		/// === 頂点リソースの作成 === ///

		//リソースを作成
		vertexResource_ = spriteCommon_->GetDxCommon()->CreateBufferResource(sizeof(VertexData) * 4);

		//リソースの先頭のアドレスを取得する
		vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();

		//使用するリソースのサイズを設定
		vertexBufferView_.SizeInBytes = sizeof(VertexData) * 4;

		//1頂点当たりのサイズを設定
		vertexBufferView_.StrideInBytes = sizeof(VertexData);

		//書き込むためのアドレスを取得する
		vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

		/// === 頂点インデックスリソースの作成 === ///

		//リソースを作成
		IndexResource_ = spriteCommon_->GetDxCommon()->CreateBufferResource(sizeof(uint32_t) * 6);

		//リソースの先頭のアドレスを取得する
		indexBufferView_.BufferLocation = IndexResource_->GetGPUVirtualAddress();

		//使用するリソースのサイズを設定
		indexBufferView_.SizeInBytes = sizeof(uint32_t) * 6;

		//フォーマットを設定
		indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

		//書き込むためのアドレスを取得する
		IndexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));

		//頂点インデックスのデータを書き込む
		indexData_[0] = 0;
		indexData_[1] = 1;
		indexData_[2] = 2;
		indexData_[3] = 1;
		indexData_[4] = 3;
		indexData_[5] = 2;

		/// === マテリアルリソースの作成 === ///

		//リソースを作成
		materialResource_ = spriteCommon_->GetDxCommon()->CreateBufferResource(sizeof(Material));

		//書き込むためのアドレスを取得する
		materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

		//マテリアルデータの設定
		materialData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		materialData_->enableLighting = false;
		materialData_->enableEdit = false;
		materialData_->uvTransform = MakeIdentity4x4();
		materialData_->ratio = 1.0f;
		materialData_->brightness = 0.1f;

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

		//座標の初期化
		translation_ = Vector2(0.0f, 0.0f);

		//角度の初期化
		rotation_ = 0.0f;

		//アンカーポイントの初期化
		anchorPoint_ = Vector2(0.5f, 0.5f);

		//X軸の反転フラグの初期化
		isFlipX_ = false;

		//Y軸の反転フラグの初期化
		isFlipY_ = false;

		//テクスチャの左上座標の初期化
		textureLeftTop_ = Vector2(0.0f, 0.0f);

	}

	///=====================================================/// 
	/// 更新
	///=====================================================///
	void Sprite::Update() {

		/// === アンカーポイントに基づいて四隅の頂点位置を計算 === ///

		//アンカーポイントから四点を計算
		float left = 0.0f - anchorPoint_.x;
		float right = 1.0f - anchorPoint_.x;
		float top = 0.0f - anchorPoint_.y;
		float bottom = 1.0f - anchorPoint_.y;

		/// === X軸・Y軸反転フラグを反映 === ///

		//X軸の反転
		if (isFlipX_) {
			left = -left;
			right = -right;
		}

		//Y軸の反転
		if (isFlipY_) {
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

		/// === 計算した頂点位置とUVをvertexData_に書き込む === ///

		//左下
		vertexData_[0].position = { left,bottom,0.0f,1.0f };
		vertexData_[0].texcoord = { texLeft,texBottom };

		//左上
		vertexData_[1].position = { left,top,0.0f,1.0f };
		vertexData_[1].texcoord = { texLeft,texTop };

		//右下
		vertexData_[2].position = { right,bottom,0.0f,1.0f };
		vertexData_[2].texcoord = { texRight,texBottom };

		//右上
		vertexData_[3].position = { right,top,0.0f,1.0f };
		vertexData_[3].texcoord = { texRight,texTop };
	}

	///=====================================================/// 
	/// 描画
	///=====================================================///
	void Sprite::Draw() {

		std::string fileName = "Resource/Sprite/" + fileName_ + "/" + texturePaths_[currentTextureIndex_];

		//頂点データの設定
		spriteCommon_->GetDxCommon()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);

		//頂点番号データの設定
		spriteCommon_->GetDxCommon()->GetCommandList()->IASetIndexBuffer(&indexBufferView_);

		//マテリアルデータの設定
		spriteCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());

		//テクスチャの設定
		spriteCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(fileName));

		//描画命令
		spriteCommon_->GetDxCommon()->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
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

		ImGui::DragFloat2("Position", &translation_.x, 1.0f);
		ImGui::SliderAngle("Rotation", &rotation_);
		ImGui::DragFloat2("Size", &size_.x, 0.1f);
		ImGui::ColorEdit4("Color", &materialData_->color.x);
		ImGui::DragFloat2("AnchorPoint", &anchorPoint_.x, 0.01f, 0.0f, 1.0f);
		ImGui::Checkbox("IsFlipX", &isFlipX_);
		ImGui::Checkbox("IsFlipY", &isFlipY_);
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

		//スプライトのサイズをテクスチャのサイズに設定
		size_ = textureSize_;
	}
}