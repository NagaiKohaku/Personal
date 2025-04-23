#pragma once

#include "d3d12.h"

#include "wrl.h"
#include "vector"

class DirectXCommon;

class SrvManager;

class OffScreen {

public:

	void Initialize();

	void PreDraw();

	void PostDraw();

private:

	void CreateRootSignature();

	void CreatePipeline();

	void CreateRenderTargetView();

	void CreateShaderResourceView();

	Microsoft::WRL::ComPtr<ID3D12Resource> CreateRenderTexture(
		Microsoft::WRL::ComPtr<ID3D12Device> device,
		uint32_t width,
		uint32_t height,
		DXGI_FORMAT format,
		const Vector4& clearColor
	);

private:

	DirectXCommon* dxCommon_ = nullptr;

	SrvManager* srvManager_ = nullptr;

	//オフスクリーン用のリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> offScreenResrouce_;

	D3D12_CPU_DESCRIPTOR_HANDLE offScreenRTVHandle_;

	//ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> offScreenRootSignature_ = nullptr;

	//グラフィックパイプラインステート
	std::vector<Microsoft::WRL::ComPtr<ID3D12PipelineState>> offScreenGraphicsPipelineState_;

	Vector4 offScreenClearColor_ = { 1.0f,0.0f,0.0f,1.0f };

	uint32_t srvIndex_ = 0;
};

