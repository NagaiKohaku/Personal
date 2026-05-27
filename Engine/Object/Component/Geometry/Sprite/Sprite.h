#pragma once

#include <Base/DirectXCommon.h>
#include <Math/Vector/Vector2.h>
#include <Math/Vector/Vector3.h>
#include <Math/Vector/Vector4.h>
#include <Math/Matrix/Matrix4x4.h>

#include "DirectXTex.h"
#include "d3d12.h"

#include "stdint.h"
#include "wrl.h"
#include "string"

namespace MyEngine {

	/// <summary>
	/// 2Dスプライトを表すクラス。
	/// </summary>
	class Sprite {

		///-------------------------------------------/// 
		/// メンバ構造体
		///-------------------------------------------///
	private:

		//頂点データ
		struct VertexData {
			Vector4 position;
			Vector2 texcoord;
			Vector3 normal;
		};

		//マテリアル
		struct Material {
			Vector4 color;
			int32_t enableLighting;
			int32_t enableEdit;
			float padding[2];
			Matrix4x4 uvTransform;
			float ratio;
			float brightness;
			float padding2[2];
		};

		struct SpriteConfig {
			Vector4 color       = { 1.0f,1.0f,1.0f,1.0f };
			Vector2 anchorPoint = { 0.5f,0.5f };
			float ratio         = 1.0f;
			float brightness    = 0.1f;
			bool enableLighting = false;
			bool enableEdit     = false;
			bool isFlipX        = false;
			bool isFlipY        = false;
		};

		///-------------------------------------------/// 
		/// メンバ関数
		///-------------------------------------------///
	public:

		/// <summary>
		/// スプライトの描画に必要なリソースを初期化します。
		/// </summary>
		/// <param name="fileName">ファイル名(.png切り捨て)</param>
		void Initialize(DirectXCommon* directCommonPtr, const std::string& fileName);

		/// <summary>
		/// スプライトの頂点データを更新します。
		/// </summary>
		void Update();

		/// <summary>
		/// スプライトを描画します。
		/// </summary>
		void Draw();

		/// <summary>
		/// 現在のテクスチャを次のものに切り替えます。
		/// </summary>
		void NextTexture();

		/// <summary>
		/// ImGuiを使用してスプライトの描画パラメータを編集します。
		/// </summary>
		void DisplayImGui();

		///-------------------------------------------/// 
		/// クラス内処理関数
		///-------------------------------------------///
	private:

		/// <summary>
		/// スプライトのサイズを現在のテクスチャの解像度に合わせます。
		/// </summary>
		void AdjustTextureSize();

		///-------------------------------------------/// 
		/// メンバ変数
		///-------------------------------------------///
	private:

		//スプライト基底
		DirectXCommon* directCommon_ = nullptr;

		/// === バッファリソース === ///

		//マテリアルリソース
		Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_ = nullptr;

		/// === リソースデータ === ///

		//マテリアルデータ
		Material* materialData_ = nullptr;

		/// === その他変数 === ///

		//ファイル名
		std::string fileName_;

		//テクスチャリスト
		std::vector<std::string> texturePaths_;

		//現在のテクスチャ番号
		int currentTextureIndex_ = 0;

		//テクスチャの左上座標
		Vector2 textureLeftTop_;

		//テクスチャのサイズ
		Vector2 textureSize_;

		SpriteConfig config_;

		///-------------------------------------------/// 
		/// セッター・ゲッター
		///-------------------------------------------///
	public:

		/// <summary>
		/// テクスチャのファイルパスを取得
		/// </summary>
		/// <returns>テクスチャのファイルパス</returns>
		const std::string GetFileName() const { return fileName_; }

		/// <summary>
		/// テクスチャのサイズを取得
		/// </summary>
		/// <returns>テクスチャのサイズ</returns>
		const Vector2& GetTextureSize() const { return textureSize_; }

		const SpriteConfig& GetConfig() const { return config_; }

		Sprite& SetColor(const Vector4& color);

		Sprite& SetAnchorPoint(const Vector2& anchorPoint);

		Sprite& SetRatio(const float& ratio);

		Sprite& SetBrightness(const float& brightness);

		Sprite& SetEnableLighting(bool enableLighting);

		Sprite& SetEnableEdit(bool enableEdit);

		Sprite& SetIsFlipX(bool isFlipX);

		Sprite& SetIsFlipY(bool isFlipY);

	};
}