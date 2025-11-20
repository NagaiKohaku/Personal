#pragma once

#include "d3d12.h"
#include "cstdint"
#include "wrl.h"

/// === 前方宣言 === ///
class DirectXCommon;

/// <summary>
/// 描画用レンダーターゲットビュー(RTV)の管理を行うクラスです。
/// </summary>
/// <remarks>
/// - シングルトンパターンで実装され、アプリケーション全体で 1 つのインスタンスを共有します。  
/// - RTV用のデスクリプタヒープを生成し、各RTVに対するCPU/GPUデスクリプタを管理します。  
/// - 最大数を設定しており、使用可能かどうかをチェックしながらRTV番号を割り当てます。  
/// - 指定したリソースに対してRTVを作成し、描画パイプラインで使用可能にします。  
/// - CPUおよびGPU用のデスクリプタハンドルを取得可能で、描画コマンド設定時に利用できます。  
/// </remarks>
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
	/// <remarks>
	/// 返り値に静的インスタンスを返します。
	/// </remarks>
	static RTVManager* GetInstance();

	/// <summary>
	/// RTV用のデスクリプタヒープを初期化します。
	/// </summary>
	/// <remarks>
	/// - DirectXCommonのインスタンスを取得します。  
	/// - 指定した最大数のRTV用デスクリプタヒープを生成します。  
	/// - ヒープ内の1つ分のデスクリプタサイズを取得して保持します。  
	/// </remarks>
	void Initialize();

	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDraw();

	/// <summary>
	/// RTVの番号を割り当てます。
	/// </summary>
	/// <returns>割り当てられたRTV番号</returns>
	/// <remarks>
	/// - 現在使用中のRTV番号が最大数を超えていないか確認します。  
	/// - 現在のRTV番号を返却し、次回の割り当て用にインデックスを増加させます。  
	/// </remarks>
	uint32_t Allocate();

	/// <summary>
	/// RTVを割り当て可能かどうかを確認します。
	/// </summary>
	/// <returns>true: 割り当て可能 / false: 割り当て不可</returns>
	/// <remarks>
	/// - 現在の使用中インデックスが最大数を超えていないかをチェックします。  
	/// - 最大数に達していなければ true を返し、超えていれば false を返します。  
	/// </remarks>
	bool AllocateCheck();

	/// <summary>
	/// 指定したリソースにRTVを作成します。
	/// </summary>
	/// <remarks>
	/// - 指定したRTVインデックスに対応するハンドルを取得します。  
	/// - RTVのフォーマットをSRGBに設定し、2Dテクスチャとしてビューを作成します。  
	/// - DirectXのCreateRenderTargetView関数を使ってリソースにRTVを割り当てます。  
	/// </remarks>
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