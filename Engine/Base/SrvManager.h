#pragma once

#include <Base/DirectXCommon.h>

#include "d3d12.h"

#include "cstdint"
#include "wrl.h"
#include "queue"

/// <summary>
/// シェーダーリソースビューを管理する仕組みです。
/// </summary>
class SrvManager {

	///-------------------------------------------/// 
	/// 静的メンバ変数
	///-------------------------------------------///
public:

	//最大SRV数
	static const uint32_t kMaxSRVCount_ = 512;

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// SrvManagerのシングルトンインスタンスを取得します。
	/// </summary>
	static SrvManager* GetInstance();

	/// <summary>
	/// SRV用のデスクリプタヒープを初期化します。
	/// </summary>
	void Initialize();

	/// <summary>
	/// 描画前にSRV用のデスクリプタヒープをコマンドリストに設定します。
	/// </summary>
	void PreDraw();

	/// <summary>
	/// SRV番号を割り当てます。
	/// </summary>
	uint32_t Allocate();

	/// <summary>
	/// SRVを割り当て可能かどうかを確認します。
	/// </summary>
	/// <returns>true: 割り当て可能 / false: 割り当て不可</returns>
	bool AllocateCheck();

	/// <summary>
	/// 解放されたメモリ番号を記録します。
	/// </summary>
	void RecordFreeIndex(uint32_t index);

	/// <summary>
	/// 2Dテクスチャ用のシェーダーリソースビュー(SRV)を生成します。
	/// </summary>
	void CreateSRVForTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT Format, UINT MipLevels);

	/// <summary>
	/// 構造化バッファ用のシェーダーリソースビュー(SRV)を生成します。
	/// </summary>
	void CreateSRVForStructuredBuffer(uint32_t srvIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride);

	/// <summary>
	/// レンダーターゲット用のシェーダーリソースビュー(SRV)を生成します。
	/// </summary>
	void CreateRenderTargetSRV(uint32_t srvIndex, ID3D12Resource* pResource);

	/// <summary>
	/// 深度テクスチャ用のシェーダーリソースビュー(SRV)を生成します。
	/// </summary>
	void CreateDepthTextureSRV(uint32_t srvIndex, ID3D12Resource* pResource);

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//DirectX基底
	MyEngine::DirectXCommon* directXCommon = nullptr;

	//解放済みSRV番号
	std::queue<uint32_t> freeIndices_;

	//現在のSRV番号
	uint32_t currentIndex_ = 0;

	//SRV用のデスクリプタサイズ
	uint32_t srvDescriptorSize_;

	//SRVデスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap_;

	///-------------------------------------------/// 
	/// ゲッター・セッター
	///-------------------------------------------///
public:

	/// <summary>
	/// CPUデスクリプターを取得
	/// </summary>
	/// <param name="index">デスクリプタの番号</param>
	/// <returns>CPUデスクリプタ</returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(uint32_t index);

	/// <summary>
	/// GPUデスクリプターを取得
	/// </summary>
	/// <param name="index">デスクリプタ番号</param>
	/// <returns>GPUデスクリプタ</returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(uint32_t index);

	/// <summary>
	/// デスクリプタテーブルの設定
	/// </summary>
	/// <param name="RootParameterIndex">パラメータ番号</param>
	/// <param name="srvIndex">SRV番号</param>
	void SetGraphicsRootDescriptorTable(UINT RootParameterIndex, uint32_t srvIndex);
};