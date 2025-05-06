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

	static OffScreen* GetInstance();

	void Initialize();

	void PreDraw();

	void PostDraw();

	void DrawToSwapChain();

private:

	void CreateRootSignature();

	void CreatePipeline();

	void CreateRenderTargetView();

	void CreateDepthStencilView();

	void CreateShaderResourceView();

	Microsoft::WRL::ComPtr<ID3D12Resource> CreateRenderTexture(
		Microsoft::WRL::ComPtr<ID3D12Device> device,
		uint32_t width,
		uint32_t height,
		DXGI_FORMAT format,
		const Vector4& clearColor
	);

	Microsoft::WRL::ComPtr<ID3D12Resource> CreateDepthStencilBuffer(
		Microsoft::WRL::ComPtr<ID3D12Device> device,
		uint32_t width,
		uint32_t height
	);

private:

	DirectXCommon* dxCommon_ = nullptr;

	RTVManager* rtvManager_ = nullptr;

	DSVManager* dsvManager_ = nullptr;

	SrvManager* srvManager_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> renderTextureResrouce_;

	Microsoft::WRL::ComPtr<ID3D12Resource> offScreenDSVResrouce_;

	D3D12_CPU_DESCRIPTOR_HANDLE offScreenRTVHandle_;

	uint32_t rtvIndex_ = 0;

	D3D12_CPU_DESCRIPTOR_HANDLE offScreenDSVHandle_;

	uint32_t dsvIndex_ = 0;

	D3D12_CPU_DESCRIPTOR_HANDLE offScreenSRVHandle_;

	uint32_t srvIndex_ = 0;

	//ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> offScreenRootSignature_ = nullptr;

	//グラフィックパイプラインステート
	std::vector<Microsoft::WRL::ComPtr<ID3D12PipelineState>> offScreenGraphicsPipelineState_;

	Vector4 offScreenClearColor_ = { 1.0f,0.0f,0.0f,1.0f };
};

