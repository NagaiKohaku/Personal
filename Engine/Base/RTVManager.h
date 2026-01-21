#pragma once

#include "d3d12.h"
#include "cstdint"
#include "wrl.h"

/// === 前方宣言 === ///
class DirectXCommon;

/// <summary>
/// 描画用レンダーターゲットビュー(RTV)の管理を行うクラスです。
/// </summary>
class RTVManager {

	///-------------------------------------------/// 
	/// 静的メンバ変数
	///-------------------------------------------///
public:

	//最大RTV数
	static const uint32_t kMaxRTVCount_ = 3;

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// RTVManagerのシングルトンインスタンスを取得します。
	/// </summary>
	static RTVManager* GetInstance();

	/// <summary>
	/// RTV用のデスクリプタヒープを初期化します。
	/// </summary>
	void Initialize();

	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDraw();

	/// <summary>
	/// RTVの番号を割り当てます。
	/// </summary>
	/// <returns>割り当てられたRTV番号</returns>
	uint32_t Allocate();

	/// <summary>
	/// RTVを割り当て可能かどうかを確認します。
	/// </summary>
	/// <returns>true: 割り当て可能 / false: 割り当て不可</returns>
	bool AllocateCheck();

	/// <summary>
	/// 指定したリソースにRTVを作成します。
	/// </summary>
	void CreateRenderTargetView(uint32_t rtvIndex, ID3D12Resource* pResource);

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//DirectX基底
	DirectXCommon* directXCommon = nullptr;

	//現在のSRV番号
	uint32_t useIndex_ = 0;

	//RTV用のデスクリプタサイズ
	uint32_t rtvDescriptorSize_;

	//RTVデスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvDescriptorHeap_;

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

};