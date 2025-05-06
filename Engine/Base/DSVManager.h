#pragma once
#include "d3d12.h"
#include "cstdint"
#include "wrl.h"

/// === 前方宣言 === ///
class DirectXCommon;

class DSVManager {

	///-------------------------------------------/// 
	/// 静的メンバ変数
	///-------------------------------------------///
public:

	//最大DSV数
	static const uint32_t kMaxDSVCount_ = 3;

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// シングルトンインスタンス
	/// </summary>
	/// <returns>インスタンス</returns>
	static DSVManager* GetInstance();

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

	void CreateDepthStencilView(uint32_t dsvIndex, ID3D12Resource* pResource);

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

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//DirectX基底
	DirectXCommon* directXCommon = nullptr;

	//現在のSRV番号
	uint32_t useIndex_ = 0;

	//DSV用のデスクリプタサイズ
	uint32_t dsvDescriptorSize_;

	//DSVデスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvDescriptorHeap_;

};