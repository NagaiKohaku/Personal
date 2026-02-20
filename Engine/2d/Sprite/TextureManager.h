#pragma once

#include "Base/DirectXCommon.h"

#include "Base/SrvManager.h"

#include "DirectXTex.h"
#include "d3d12.h"
#include "d3dx12.h"

#include "unordered_map"
#include "string"
#include "vector"
#include "wrl.h"

namespace MyEngine {

	/// <summary>
	/// テクスチャ管理を行うシングルトンクラス。
	/// </summary>
	class TextureManager {

		///-------------------------------------------/// 
		/// 静的メンバ変数
		///-------------------------------------------///
	public:

		//SRV番号の現在の総数
		static uint32_t kSRVIndexTop;

		///-------------------------------------------/// 
		/// メンバ構造体
		///-------------------------------------------///
	private:

		//テクスチャデータ
		struct TextureData {
			std::string modelName;
			DirectX::TexMetadata metaData;
			Microsoft::WRL::ComPtr<ID3D12Resource> resource;
			Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource;
			uint32_t srvIndex;
			D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU;
			D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU;
		};

		///-------------------------------------------/// 
		/// メンバ関数
		///-------------------------------------------///
	public:

		/// <summary>
		/// TextureManagerのシングルトンインスタンスを取得します。
		/// </summary>
		static TextureManager* GetInstance();

		/// <summary>
		/// TextureManagerを初期化します。
		/// </summary>
		void Initialize(DirectXCommon* dxCommonPtr);

		/// <summary>
		/// 指定されたファイルパスのテクスチャを読み込み、GPU上に展開します。
		/// </summary>
		/// <param name="filePath"> 読み込むテクスチャファイルのパス </param>
		void LoadTexture(const std::string& filePath);

		/// <summary>
		/// 指定されたファイルパスのキューブマップテクスチャを読み込み、GPU上に展開します。
		/// </summary>
		/// <param name="filePath"> 読み込むキューブマップテクスチャファイルのパス </param>
		void LoadCubeTexture(const std::string& filePath);

		///-------------------------------------------/// 
		/// クラス内処理関数
		///-------------------------------------------///
	private:

		/// <summary>
		/// 指定されたテクスチャメタデータを基に、DirectX 12 のテクスチャリソースを生成します。
		/// </summary>
		/// <param name="metadata"> テクスチャのメタデータ </param>
		/// <returns> 生成されたID3D12Resourceを保持するComPtrオブジェクト </returns>
		Microsoft::WRL::ComPtr<ID3D12Resource> CreateTextureResource(const DirectX::TexMetadata& metadata);

		/// <summary>
		/// 指定されたメタデータを基に、キューブマップ用のテクスチャリソースを生成します。
		/// </summary>
		/// <param name="metadata"> テクスチャのメタデータ </param>
		/// <returns> 生成されたキューブマップ用ID3D12Resourceを保持するComPtrオブジェクト </returns>
		Microsoft::WRL::ComPtr<ID3D12Resource> CreateCubeTextureResource(const DirectX::TexMetadata& metadata);

		/// <summary>
		/// テクスチャデータをGPUにアップロードし、GPUで使用可能な状態にします。
		/// </summary>
		/// <param name="textureData"> GPU上に作成されたテクスチャリソース </param>
		/// <param name="mipImages"> 読み込んだ画像およびミップマップ情報を保持するScratchImage構造体 </param>
		/// <returns> GPU転送用の中間リソース </returns>
		[[nodiscard]]
		Microsoft::WRL::ComPtr<ID3D12Resource> UploadTextureData(
			Microsoft::WRL::ComPtr<ID3D12Resource> textureData,
			const DirectX::ScratchImage& mipImages
		);

		/// <summary>
		/// キューブマップ用のテクスチャデータをGPUにアップロードし、使用可能な状態にします。
		/// </summary>
		/// <param name="textureData"> GPU上に作成されたキューブマップテクスチャリソース </param>
		/// <param name="mipImages"> 読み込んだキューブマップ画像およびミップマップ情報を保持するScratchImage構造体 </param>
		/// <returns> GPU転送用の中間リソース </returns>
		[[nodiscard]]
		Microsoft::WRL::ComPtr<ID3D12Resource> UploadCubeTextureData(
			Microsoft::WRL::ComPtr<ID3D12Resource> textureData,
			const DirectX::ScratchImage& mipImages
		);

		///-------------------------------------------/// 
		/// メンバ変数
		///-------------------------------------------///
	private:

		//DirectX基底
		DirectXCommon* dxCommon_ = nullptr;

		//SRVマネージャー
		SrvManager* srvManager_ = nullptr;

		//テクスチャデータリスト
		std::unordered_map<std::string, TextureData> textureData_;

		///-------------------------------------------/// 
		/// ゲッター・セッター
		///-------------------------------------------///
	public:

		/// <summary>
		/// メタデータを取得
		/// </summary>
		/// <param name="modelName">ファイルパス</param>
		/// <returns>メタデータ</returns>
		const DirectX::TexMetadata& GetMetaData(const std::string& filePath);

		/// <summary>
		/// SRV番号を取得
		/// </summary>
		/// <param name="modelName">ファイルパス</param>
		/// <returns>SRV番号</returns>
		uint32_t GetSrvIndex(const std::string& filePath);

		/// <summary>
		/// GPUデスクリプタを取得
		/// </summary>
		/// <param name="modelName">ファイルパス</param>
		/// <returns>GPUデスクリプタ</returns>
		D3D12_GPU_DESCRIPTOR_HANDLE GetSrvHandleGPU(const std::string& filePath);

	};
}