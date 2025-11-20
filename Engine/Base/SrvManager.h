#pragma once

#include "d3d12.h"

#include "cstdint"
#include "wrl.h"
#include "queue"

/// === 前方宣言 === ///
class DirectXCommon;

/// <summary>
/// シェーダーリソースビューを管理する仕組みです。
/// </summary>
/// <remarks>
/// - シングルトンパターンで実装され、アプリケーション全体で 1 つのインスタンスを共有します。  
/// - グラフィックス描画で使用するリソースビューを保持するための専用領域を用意します。  
/// - 使用後に解放された番号は記録され、将来的な割り当てで再利用されます。  
/// - 2D画像データや構造化データ、描画先用テクスチャ、深度情報用テクスチャなど、さまざまな形式のリソースビューを作成できます。  
/// - ゲッターを通じてCPU/GPUデスクリプタを取得可能です。  
/// </remarks>
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
	/// <remarks>
	/// 返り値に静的インスタンスを返します。
	/// </remarks>
	static SrvManager* GetInstance();

	/// <summary>
	/// SRV用のデスクリプタヒープを初期化します。
	/// </summary>
	/// <remarks>
	/// - DirectXCommonのインスタンスを取得します。  
	/// - 指定した最大数のSRV用デスクリプタヒープを生成します。  
	/// - ヒープ内の1つ分のデスクリプタサイズを取得して保持します。  
	/// </remarks>
	void Initialize();

	/// <summary>
	/// 描画前にSRV用のデスクリプタヒープをコマンドリストに設定します。
	/// </summary>
	/// <remarks>
	/// - 描画に使用するSRVデスクリプタヒープを配列にまとめます。  
	/// - コマンドリストのSetDescriptorHeaps関数を呼び出してSRVヒープを設定します。  
	/// </remarks>
	void PreDraw();

	/// <summary>
	/// SRV番号を割り当てます。
	/// </summary>
	/// <remarks>
	/// - すでに解放された番号が存在する場合は、それを再利用します。  
	/// - 解放された番号がない場合は、新しい番号を順番に割り当てます。  
	/// - 割り当て可能な最大数を超えないように確認します。  
	/// </remarks>
	uint32_t Allocate();

	/// <summary>
	/// SRVを割り当て可能かどうかを確認します。
	/// </summary>
	/// <returns>true: 割り当て可能 / false: 割り当て不可</returns>
	/// <remarks>
	/// - 現在の使用中インデックスが最大数を超えていないかをチェックします。  
	/// - 最大数に達していなければ true を返し、超えていれば false を返します。  
	/// </remarks>
	bool AllocateCheck();

	/// <summary>
	/// 解放されたメモリ番号を記録します。
	/// </summary>
	/// <remarks>
	/// - 解放された番号は内部の管理リストに追加されます。  
	/// - 将来の割り当て時にこの番号が再利用されます。  
	/// </remarks>
	void RecordFreeIndex(uint32_t index);

	/// <summary>
	/// 2Dテクスチャ用のシェーダーリソースビュー(SRV)を生成します。
	/// </summary>
	/// <remarks>
	/// - 指定されたテクスチャリソースに対してSRVを作成します。  
	/// - SRVのフォーマットとミップマップレベルを設定します。  
	/// - 作成したSRVは指定された番号で管理されます。  
	/// </remarks>
	void CreateSRVForTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT Format, UINT MipLevels);

	/// <summary>
	/// 構造化バッファ用のシェーダーリソースビュー(SRV)を生成します。
	/// </summary>
	/// <remarks>
	/// - 指定されたバッファリソースに対してSRVを作成します。  
	/// - バッファの要素数と1要素あたりのバイトサイズを設定します。  
	/// - SRVのフォーマットは不明として扱い、シェーダーで構造化バッファとしてアクセス可能にします。  
	/// - 作成したSRVは指定された番号で管理されます。  
	/// </remarks>
	void CreateSRVForStructuredBuffer(uint32_t srvIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride);

	/// <summary>
	/// レンダーターゲット用のシェーダーリソースビュー(SRV)を生成します。
	/// </summary>
	/// <remarks>
	/// - 指定された2Dテクスチャリソースに対してSRVを作成します。  
	/// - 色のフォーマットをsRGBの8ビット4チャンネルに設定します。  
	/// - シェーダーからテクスチャとしてアクセス可能にします。  
	/// - 作成したSRVは指定された番号で管理されます。  
	/// </remarks>
	void CreateRenderTargetSRV(uint32_t srvIndex, ID3D12Resource* pResource);

	/// <summary>
	/// 深度テクスチャ用のシェーダーリソースビュー(SRV)を生成します。
	/// </summary>
	/// <remarks>
	/// - 指定された2D深度テクスチャリソースに対してSRVを作成します。  
	/// - フォーマットを深度専用の24ビットに設定します。  
	/// - シェーダーからテクスチャとしてアクセス可能にします。  
	/// - 作成したSRVは指定された番号で管理されます。  
	/// </remarks>
	void CreateDepthTextureSRV(uint32_t srvIndex, ID3D12Resource* pResource);

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//DirectX基底
	DirectXCommon* directXCommon = nullptr;

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