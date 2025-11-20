#pragma once
#include "d3d12.h"
#include "cstdint"
#include "wrl.h"

/// === 前方宣言 === ///
class DirectXCommon;

/// <summary>
/// DirectX12 の深度ステンシルビュー (DSV) を管理するクラスです。
/// </summary>
/// <remarks>
/// - シングルトンパターンで実装され、アプリケーション全体で 1 つのインスタンスを共有します。  
/// - DSV用のデスクリプタヒープを生成・管理し、CPU/GPU用ハンドルを提供します。  
/// - DSVの割り当てや利用可能チェック、インデックス管理を行います。  
/// - 指定されたリソースに対して標準的な深度ステンシルビューを生成可能です。  
/// - 32ビット浮動小数点形式の深度テクスチャ用DSVも生成できます。  
/// - DirectXCommon インスタンスを利用して、デバイス経由でDSVを作成します。  
/// - ゲッターを通じてCPU/GPUデスクリプタを取得可能です。  
/// </remarks>
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
	/// <remarks>
	/// 返り値に静的インスタンスを返します。
	/// </remarks>
	static DSVManager* GetInstance();

	/// <summary>
	/// DSV用のデスクリプタヒープを初期化します。
	/// </summary>
	/// <remarks>
	/// - DirectXCommonのインスタンスを取得します。  
	/// - 指定した最大数のDSV用デスクリプタヒープを生成します。  
	/// - ヒープ内の1つ分のデスクリプタサイズを取得して保持します。  
	/// </remarks>
	void Initialize();

	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDraw();

	/// <summary>
	/// DSVの番号を割り当てます。
	/// </summary>
	/// <returns>割り当てられたDSV番号</returns>
	/// <remarks>
	/// - 現在使用中のDSV番号が最大数を超えていないか確認します。  
	/// - 現在のDSV番号を返却し、次回の割り当て用にインデックスを増加させます。  
	/// </remarks>
	uint32_t Allocate();

	/// <summary>
	/// DSVを割り当て可能かどうかを確認します。
	/// </summary>
	/// <returns>true: 割り当て可能 / false: 割り当て不可</returns>
	/// <remarks>
	/// - 現在の使用中インデックスが最大数を超えていないかをチェックします。  
	/// - 最大数に達していなければ true を返し、超えていれば false を返します。  
	/// </remarks>
	bool AllocateCheck();

	/// <summary>
	/// 指定されたリソースに対してDSVを生成します。
	/// </summary>
	/// <param name="dsvIndex">割り当て済みのDSV番号</param>
	/// <param name="pResource">DSVを作成する対象のリソース</param>
	/// <remarks>
	/// - 2Dテクスチャ用の標準的な深度ステンシルフォーマットを使用してDSVを設定します。  
	/// - DSVのディスクリプタハンドルを取得し、デバイスにCreateDepthStencilViewを呼び出してDSVを生成します。  
	/// </remarks>
	void CreateDepthStencilView(uint32_t dsvIndex, ID3D12Resource* pResource);

	/// <summary>
	/// 指定されたリソースに対して32ビット浮動小数点形式の深度ステンシルビューを生成します。
	/// </summary>
	/// <param name="dsvIndex">割り当て済みのDSV番号</param>
	/// <param name="pResource">DSVを作成する対象のリソース</param>
	/// <remarks>
	/// - 32ビット浮動小数点深度用フォーマットを使用してDSVを設定します。  
	/// - DSVのディスクリプタハンドルを取得し、デバイスにCreateDepthStencilViewを呼び出してDSVを生成します。  
	/// </remarks>
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