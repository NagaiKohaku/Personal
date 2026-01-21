#pragma once
#include "d3d12.h"
#include "cstdint"
#include "wrl.h"

/// === 前方宣言 === ///
class DirectXCommon;

/// <summary>
/// DirectX12 の深度ステンシルビュー (DSV) を管理するクラスです。
/// </summary>
class DSVManager {

	///-------------------------------------------/// 
	/// 静的メンバ変数
	///-------------------------------------------///
public:

	//最大DSV数
	static const uint32_t kMaxDSVCount_ = 2;

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// DSVManagerのシングルトンインスタンスを取得します。
	/// </summary>
	static DSVManager* GetInstance();

	/// <summary>
	/// DSV用のデスクリプタヒープを初期化します。
	/// </summary>
	void Initialize();

	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDraw();

	/// <summary>
	/// DSVの番号を割り当てます。
	/// </summary>
	/// <returns>割り当てられたDSV番号</returns>
	uint32_t Allocate();

	/// <summary>
	/// DSVを割り当て可能かどうかを確認します。
	/// </summary>
	/// <returns>true: 割り当て可能 / false: 割り当て不可</returns>
	bool AllocateCheck();

	/// <summary>
	/// 指定されたリソースに対してDSVを生成します。
	/// </summary>
	/// <param name="dsvIndex">割り当て済みのDSV番号</param>
	/// <param name="pResource">DSVを作成する対象のリソース</param>
	void CreateDepthStencilView(uint32_t dsvIndex, ID3D12Resource* pResource);

	/// <summary>
	/// 指定されたリソースに対して32ビット浮動小数点形式の深度ステンシルビューを生成します。
	/// </summary>
	/// <param name="dsvIndex">割り当て済みのDSV番号</param>
	/// <param name="pResource">DSVを作成する対象のリソース</param>
	void CreateDepthTexture(uint32_t dsvIndex, ID3D12Resource* pResource);

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