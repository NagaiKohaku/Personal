#pragma once

#include "Base/DirectXCommon.h"

#include "DirectXTex.h"
#include "d3d12.h"
#include "d3dx12.h"

#include "unordered_map"
#include "string"
#include "vector"
#include "wrl.h"

/// === 前方宣言 === ///
class SrvManager;

///=====================================================/// 
/// テクスチャマネージャー
///=====================================================///
class TextureManager {

	///-------------------------------------------/// 
	/// 静的メンバ変数
	///-------------------------------------------///
public:

	//SRV番号の現在の総数
	static uint32_t kSRVIndexTop;

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// シングルトンインスタンス
	/// </summary>
	/// <returns>インスタンス</returns>
	static TextureManager* GetInstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// テクスチャの読み込み
	/// </summary>
	/// <param name="modelName">ファイルパス</param>
	void LoadTexture(const std::string& filePath);

	///-------------------------------------------/// 
	/// ゲッター・セッター
	///-------------------------------------------///
public:

	/// <summary>
	/// メタデータのゲッター
	/// </summary>
	/// <param name="modelName">ファイルパス</param>
	/// <returns>メタデータ</returns>
	const DirectX::TexMetadata& GetMetaData(const std::string& filePath);

	/// <summary>
	/// SRV番号のゲッター
	/// </summary>
	/// <param name="modelName">ファイルパス</param>
	/// <returns>SRV番号</returns>
	uint32_t GetSrvIndex(const std::string& filePath);

	/// <summary>
	/// GPUデスクリプタのゲッター
	/// </summary>
	/// <param name="modelName">ファイルパス</param>
	/// <returns>GPUデスクリプタ</returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetSrvHandleGPU(const std::string& filePath);

	///-------------------------------------------/// 
	/// クラス内処理関数
	///-------------------------------------------///
private:

	/// <summary>
	/// テクスチャリソースの生成
	/// </summary>
	/// <param name="metadata">メタデータ</param>
	/// <returns>テクスチャリソース</returns>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateTextureResource(const DirectX::TexMetadata& metadata);

	/// <summary>
	/// テクスチャデータをGPUに転送
	/// </summary>
	/// <param name="textureData">テクスチャデータ</param>
	/// <param name="mipImages">ミップレベル</param>
	/// <returns>リソース</returns>
	[[nodiscard]]
	Microsoft::WRL::ComPtr<ID3D12Resource> UploadTextureData(
		Microsoft::WRL::ComPtr<ID3D12Resource> textureData,
		const DirectX::ScratchImage& mipImages
	);

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
	/// メンバ変数
	///-------------------------------------------///
private:

	//DirectX基底
	DirectXCommon* directXCommon_ = nullptr;

	//SRVマネージャー
	SrvManager* srvManager_ = nullptr;

	//テクスチャデータリスト
	std::unordered_map<std::string, TextureData> textureDatas;

};