#pragma once

#include "d3d12.h"

#include "Math/Vector4.h"

#include "wrl.h"
#include "vector"
#include "cstdint"

class DirectXCommon;

class RTVManager;

class DSVManager;

class SrvManager;

class OffScreen {

public:

	/// <summary>
	/// シングルトンインスタンス
	/// </summary>
	/// <returns>インスタンス</returns>
	static OffScreen* GetInstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDraw();

	/// <summary>
	/// 描画後処理
	/// </summary>
	void PostDraw();

	/// <summary>
	/// 描画結果をSwapChainにコピー
	/// </summary>
	void DrawToSwapChain();

private:

	/// <summary>
	/// ルートシグネチャの生成
	/// </summary>
	void CreateRootSignature();

	/// <summary>
	/// PSOを生成
	/// </summary>
	void CreatePipeline();

	/// <summary>
	/// RTVを生成
	/// </summary>
	void CreateRenderTargetView();

	/// <summary>
	/// DSVを生成
	/// </summary>
	void CreateDepthStencilView();

	/// <summary>
	/// SRVを生成
	/// </summary>
	void CreateShaderResourceView();

	/// <summary>
	/// OffScreen用のテクスチャを生成
	/// </summary>
	/// <param name="device"></param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <param name="format"></param>
	/// <param name="clearColor"></param>
	/// <returns></returns>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateRenderTexture(
		Microsoft::WRL::ComPtr<ID3D12Device> device,
		uint32_t width,
		uint32_t height,
		DXGI_FORMAT format,
		const Vector4& clearColor
	);

	/// <summary>
	/// 深度ステンシルバッファを生成
	/// </summary>
	/// <param name="device"></param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <returns></returns>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateDepthStencilBuffer(
		Microsoft::WRL::ComPtr<ID3D12Device> device,
		uint32_t width,
		uint32_t height
	);

private:

	//DirectX基底
	DirectXCommon* dxCommon_ = nullptr;

	//RTVマネージャー
	RTVManager* rtvManager_ = nullptr;

	//DSVマネージャー
	DSVManager* dsvManager_ = nullptr;

	//SRVマネージャー
	SrvManager* srvManager_ = nullptr;

	//オフスクリーン用のテクスチャ
	Microsoft::WRL::ComPtr<ID3D12Resource> renderTextureResrouce_;

	//オフスクリーン用のDSV
	Microsoft::WRL::ComPtr<ID3D12Resource> offScreenDSVResrouce_;

	//RTVハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE offScreenRTVHandle_;

	//RTVのメモリ番号
	uint32_t rtvIndex_ = 0;

	//DSVハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE offScreenDSVHandle_;

	//DSVのメモリ番号
	uint32_t dsvIndex_ = 0;

	//SRVハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE offScreenSRVHandle_;

	//SRVのメモリ番号
	uint32_t srvIndex_ = 0;

	//ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> offScreenRootSignature_ = nullptr;

	//グラフィックパイプラインステート
	std::vector<Microsoft::WRL::ComPtr<ID3D12PipelineState>> offScreenGraphicsPipelineState_;

	//オフスクリーンのクリア値
	Vector4 offScreenClearColor_ = { 1.0f,0.0f,0.0f,1.0f };
};

