#pragma once

#include <d3dx12.h>
#include <d3d12.h>
#include <dxcapi.h>

#include <vector>
#include <wrl.h>

namespace MyEngine {

	class DirectXCommon;
	class Camera;

	class ObjectCommonBase {

	public:

		ObjectCommonBase();

		virtual ~ObjectCommonBase();

	public:

		virtual void Initialize(DirectXCommon* dxCommonPtr);

		virtual void CommonDrawSetting();

	protected:

		virtual void CreateRootSignature() = 0;

		virtual void CreateInputElement(
			std::vector<D3D12_INPUT_ELEMENT_DESC>& inputElements
		) = 0;

		virtual void CreateShader(
			Microsoft::WRL::ComPtr<IDxcBlob>& vs,
			Microsoft::WRL::ComPtr<IDxcBlob>& ps
		) = 0;

		D3D12_DEPTH_STENCIL_DESC CreateDepthStencil();

		D3D12_RASTERIZER_DESC CreateRasterizer();

		D3D12_BLEND_DESC CreateBlend();

		void CreateGraphicsPipeline();

		void CreateDescriptorRootSignature(
			D3D12_ROOT_PARAMETER* rootParameters,
			UINT numRootParameters,
			D3D12_STATIC_SAMPLER_DESC* staticSamplers,
			UINT numStaticSamplers
		);

	protected:

		//DirectX基底
		DirectXCommon* dxCommon_ = nullptr;

		//ルートシグネチャ
		Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;

		//グラフィックパイプラインステート
		Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_;

	public:

		DirectXCommon* GetDxCommon() const { return dxCommon_; }

	};

}