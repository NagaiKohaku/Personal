#include "SRVManager.h"

#include <Base/DirectXCommon.h>

#include <cassert>

namespace MyEngine {

	SRVManager::SRVManager() = default;

	SRVManager::~SRVManager() = default;

	///=====================================================/// 
	/// SRV用のデスクリプタヒープを初期化
	///=====================================================///
	void SRVManager::Initialize(DirectXCommon* dxCommonPtr) {

		//DirextX基底のインスタンスを取得
		dxCommon_ = dxCommonPtr;

		//srvデスクリプタヒープの初期化
		descriptorHeap_ = dxCommon_->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, kMaxSRVCount_, true);

		//srvデスクリプタヒープのサイズを取得
		descriptorSize_ = dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	}

	///=====================================================/// 
	/// 描画前にSRV用のデスクリプタヒープをコマンドリストに設定
	///=====================================================///
	void SRVManager::PreDraw() {

		//描画前のDescriptorHeapの設定
		ID3D12DescriptorHeap* descriptorHeaps[] = { descriptorHeap_.Get() };

		//コマンドリストにsrvデスクリプタヒープを設定する
		dxCommon_->GetCommandList()->SetDescriptorHeaps(1, descriptorHeaps);
	}

	///=====================================================/// 
	/// 2Dテクスチャ用のシェーダーリソースビュー(SRV)を生成
	///=====================================================///
	void SRVManager::CreateSRVForTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT Format, UINT MipLevels) {

		//SRVの情報
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};

		//SRVの設定
		srvDesc.Format = Format;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = UINT(MipLevels);

		//SRVの生成
		dxCommon_->GetDevice()->CreateShaderResourceView(pResource, &srvDesc, GetCPUDescriptorHandle(srvIndex));
	}

	///=====================================================/// 
	/// 構造化バッファ用のシェーダーリソースビュー(SRV)を生成
	///=====================================================///
	void SRVManager::CreateSRVForStructuredBuffer(uint32_t srvIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride) {

		//SRVの情報
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};

		//SRVの設定
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		srvDesc.Buffer.FirstElement = 0;
		srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
		srvDesc.Buffer.NumElements = numElements;
		srvDesc.Buffer.StructureByteStride = structureByteStride;

		//SRVの生成
		dxCommon_->GetDevice()->CreateShaderResourceView(pResource, &srvDesc, GetCPUDescriptorHandle(srvIndex));
	}

	///=====================================================/// 
	/// レンダーターゲット用のシェーダーリソースビュー(SRV)を生成
	///=====================================================///
	void SRVManager::CreateRenderTargetSRV(uint32_t srvIndex, ID3D12Resource* pResource) {

		//SRVの情報
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};

		//SRVの設定
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;

		//SRVの生成
		dxCommon_->GetDevice()->CreateShaderResourceView(pResource, &srvDesc, GetCPUDescriptorHandle(srvIndex));
	}

	///=====================================================/// 
	/// 深度テクスチャ用のシェーダーリソースビュー(SRV)を生成
	///=====================================================///
	void SRVManager::CreateDepthTextureSRV(uint32_t srvIndex, ID3D12Resource* pResource) {

		//SRVの情報
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};

		//SRVの設定
		srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;

		//SRVの生成
		dxCommon_->GetDevice()->CreateShaderResourceView(pResource, &srvDesc, GetCPUDescriptorHandle(srvIndex));
	}

	///=====================================================/// 
	/// デスクリプタテーブルの設定
	///=====================================================///
	void SRVManager::SetGraphicsRootDescriptorTable(UINT RootParameterIndex, uint32_t srvIndex) {

		dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(RootParameterIndex, GetGPUDescriptorHandle(srvIndex));
	}
}