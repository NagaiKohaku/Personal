#pragma once

#include "d3d12.h"

#include "cstdint"
#include "wrl.h"

/// === 前方宣言 === ///
class DirectXCommon;

///=====================================================/// 
/// SRVマネージャー
///=====================================================///
class SrvManager {

	///-------------------------------------------/// 
	/// 静的メンバ変数
	///-------------------------------------------///
public:

	//最大SRV数(最大テクスチャ枚数)
	static const uint32_t kMaxSRVCount_ = 512;

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// シングルトンインスタンス
	/// </summary>
	/// <returns>インスタンス</returns>
	static SrvManager* GetInstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDraw();

	/// <summary>
	/// メモリ確保
	/// </summary>
	/// <returns>確保したメモリ番号</returns>
	uint32_t Allocate();

	/// <summary>
	/// メモリ確保可能チェック
	/// </summary>
	/// <returns>フラグ</returns>
	bool AllocateCheck();

	/// <summary>
	/// SRV生成(テクスチャ用)
	/// </summary>
	/// <param name="srvIndex">srv番号</param>
	/// <param name="pResource">リソース</param>
	/// <param name="Format">フォーマット</param>
	/// <param name="MipLevels">ミップレベル</param>
	void CreateSRVForTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT Format, UINT MipLevels);

	/// <summary>
	/// SRV生成(Structured Buffer用)
	/// </summary>
	/// <param name="srvIndex">srv番号</param>
	/// <param name="pResource">リソース</param>
	/// <param name="numElements">要素数</param>
	/// <param name="structureByteStride"></param>
	void CreateSRVForStructuredBuffer(uint32_t srvIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride);

	///-------------------------------------------/// 
	/// ゲッター・セッター
	///-------------------------------------------///
public:

	/// <summary>
	/// CPUデスクリプターのゲッター
	/// </summary>
	/// <param name="index">デスクリプタの番号</param>
	/// <returns>CPUデスクリプタ</returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(uint32_t index);

	/// <summary>
	/// GPUデスクリプターのゲッター
	/// </summary>
	/// <param name="index">デスクリプタ番号</param>
	/// <returns>GPUデスクリプタ</returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(uint32_t index);

	/// <summary>
	/// デスクリプタテーブルのセッター
	/// </summary>
	/// <param name="RootParameterIndex">パラメータ番号</param>
	/// <param name="srvIndex">SRV番号</param>
	void SetGraphicsRootDescriptorTable(UINT RootParameterIndex, uint32_t srvIndex);

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//DirectX基底
	DirectXCommon* directXCommon = nullptr;

	//現在のSRV番号
	uint32_t useIndex_ = 0;

	//SRV用のデスクリプタサイズ
	uint32_t srvDescriptorSize_;

	//SRVデスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap_;

};