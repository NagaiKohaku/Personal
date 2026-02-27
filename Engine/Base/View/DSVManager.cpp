#include "DSVManager.h"

#include <Base/DirectXCommon.h>

#include <cassert>

namespace MyEngine {

	DSVManager::DSVManager() = default;

	DSVManager::~DSVManager() = default;

	///=====================================================/// 
	/// DSV用のデスクリプタヒープを初期化
	///=====================================================///
	void DSVManager::Initialize(DirectXCommon* dxCommonPtr) {

		//DirectX基底のインスタンスを取得
		dxCommon_ = dxCommonPtr;

		//DSVデスクリプタヒープの初期化
		descriptorHeap_ = dxCommon_->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, kMaxDSVCount_, false);

		//DSVデスクリプタヒープのサイズを取得
		descriptorSize_ = dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	}

	///=====================================================/// 
	/// 描画前処理
	///=====================================================///
	void DSVManager::PreDraw() {
	}

	///=====================================================/// 
	/// 指定されたリソースに対してDSVを生成
	///=====================================================///
	void DSVManager::CreateDepthStencilView(uint32_t dsvIndex, ID3D12Resource* pResource) {

		//DSVの情報
		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};

		//DSVの設定
		dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;        //Format。基本的にはResourceに合わせる
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D; //2DTexture

		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = GetCPUDescriptorHandle(dsvIndex);

		//DSVHeapの先頭にDSVを作る
		dxCommon_->GetDevice()->CreateDepthStencilView(
			pResource,
			&dsvDesc,
			dsvHandle
		);

	}

	///================================================================/// 
	/// 指定されたリソースに対して32ビット浮動小数点形式の深度ステンシルビューを生成
	///================================================================///
	void DSVManager::CreateDepthTexture(uint32_t dsvIndex, ID3D12Resource* pResource) {

		//DSVの情報
		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};

		//DSVの設定
		dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;     //Format。基本的にはResourceに合わせる
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D; //2DTexture

		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = GetCPUDescriptorHandle(dsvIndex);

		//DSVHeapの先頭にDSVを作る
		dxCommon_->GetDevice()->CreateDepthStencilView(
			pResource,
			&dsvDesc,
			dsvHandle
		);
	}
}