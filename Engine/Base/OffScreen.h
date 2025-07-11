#pragma once

#include "d3d12.h"

#include "Math/Vector4.h"
#include "Math/Matrix4x4.h"

#include "wrl.h"
#include "vector"
#include "cstdint"
#include "string"

class DirectXCommon;

class RTVManager;

class DSVManager;

class SrvManager;

class Camera;

class OffScreen {

private:

	struct Material {
		Matrix4x4 projectionInverse;
	};

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

	void ImGui();

public:

	void SetDefaultCamera(Camera* ptr);

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
	/// RenderTextureのSRVを生成
	/// </summary>
	void CreateRenderTextureSRV();

	/// <summary>
	/// DepthTextureのSRVを生成
	/// </summary>
	void CreateDepthTextureSRV();

	/// <summary>
	/// RenderTextureを生成
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
	/// DepthTextureを生成
	/// </summary>
	/// <param name="device"></param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <param name="format"></param>
	/// <param name="clearColor"></param>
	/// <returns></returns>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateDepthTexture(
		Microsoft::WRL::ComPtr<ID3D12Device> device,
		uint32_t width,
		uint32_t height,
		DXGI_FORMAT format
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

	Camera* camera_ = nullptr;

	//オフスクリーン用のテクスチャ
	Microsoft::WRL::ComPtr<ID3D12Resource> renderTextureResrouce_;

	//DepthTextreのリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> depthTextureResource_;

	//オフスクリーン用のマテリアルリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_;

	//RTVハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE offScreenRTVHandle_;

	//RTVのメモリ番号
	uint32_t rtvIndex_ = 0;

	//DSVハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE offScreenDSVHandle_;

	//DSVのメモリ番号
	uint32_t dsvIndex_ = 0;

	//renderTextureハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE renderTextureSRVHandle_;

	//renderTextureのメモリ番号
	uint32_t renderTextureSRVIndex_ = 0;

	//DepthTextureハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE depthTextureSRVHandle_;

	//DepthTextureのメモリ番号
	uint32_t depthTextureSRVIndex_ = 0;

	//オフスクリーン用のマテリアルデータ
	Material* materialData_ = nullptr;

	//ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> offScreenRootSignature_ = nullptr;

	//グラフィックパイプラインステート
	std::vector<Microsoft::WRL::ComPtr<ID3D12PipelineState>> offScreenGraphicsPipelineState_;

	//現在のシェーダー名
	std::wstring currentShaderName_;

	//オフスクリーンのクリア値
	Vector4 offScreenClearColor_ = { 0.0f,0.0f,0.0f,1.0f };
};

