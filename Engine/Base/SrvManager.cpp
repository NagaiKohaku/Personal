#include "SrvManager.h"

#include "Base/DirectXCommon.h"

#include "cassert"

namespace MyEngine {

	///=====================================================/// 
	/// SrvManagerのシングルトンインスタンスを取得
	///=====================================================///
	SrvManager* SrvManager::GetInstance() {
		static SrvManager instance;
		return &instance;
	}

	///=====================================================/// 
	/// SRV用のデスクリプタヒープを初期化
	///=====================================================///
	void SrvManager::Initialize(DirectXCommon* dxCommonPtr) {

		//DirextX基底のインスタンスを取得
		dxCommon_ = dxCommonPtr;

		//srvデスクリプタヒープの初期化
		srvDescriptorHeap_ = dxCommon_->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, kMaxSRVCount_, true);

		//srvデスクリプタヒープのサイズを取得
		srvDescriptorSize_ = dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	}

	///=====================================================/// 
	/// 描画前にSRV用のデスクリプタヒープをコマンドリストに設定
	///=====================================================///
	void SrvManager::PreDraw() {

		//描画前のDescriptorHeapの設定
		ID3D12DescriptorHeap* descriptorHeaps[] = { srvDescriptorHeap_.Get() };

		//コマンドリストにsrvデスクリプタヒープを設定する
		dxCommon_->GetCommandList()->SetDescriptorHeaps(1, descriptorHeaps);
	}

	///=====================================================/// 
	/// SRV番号を割り当て
	///=====================================================///
	uint32_t SrvManager::Allocate() {

		//NOTE:InstancingDataを使用するオブジェクトが増えると解放されるまでに最大数に達するかも

		//SRV番号が最大数を越えていないかの確認
		assert(kMaxSRVCount_ > currentIndex_);

		//解放済みメモリがあれば
		if (!freeIndices_.empty()) {

			//メモリ番号を再利用
			uint32_t index = freeIndices_.front();

			freeIndices_.pop();

			return index;
		}

		//新しくメモリ確保
		return currentIndex_++;
	}

	///=====================================================/// 
	/// SRVを割り当て可能かどうかを確認
	///=====================================================///
	bool SrvManager::AllocateCheck() {

		//SRV番号が最大数を越えていないかの確認
		if (kMaxSRVCount_ > currentIndex_) {
			return true;
		}

		return false;
	}

	///=====================================================/// 
	/// 解放されたメモリ番号を記録
	///=====================================================///
	void SrvManager::RecordFreeIndex(uint32_t index) {

		//解放されたメモリ番号を記録
		freeIndices_.push(index);
	}

	///=====================================================/// 
	/// 2Dテクスチャ用のシェーダーリソースビュー(SRV)を生成
	///=====================================================///
	void SrvManager::CreateSRVForTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT Format, UINT MipLevels) {

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
	void SrvManager::CreateSRVForStructuredBuffer(uint32_t srvIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride) {

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
	void SrvManager::CreateRenderTargetSRV(uint32_t srvIndex, ID3D12Resource* pResource) {

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
	void SrvManager::CreateDepthTextureSRV(uint32_t srvIndex, ID3D12Resource* pResource) {

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
	/// CPUデスクリプターを取得
	///=====================================================///
	D3D12_CPU_DESCRIPTOR_HANDLE SrvManager::GetCPUDescriptorHandle(uint32_t index) {

		//デスクリプタの最初のメモリを取得
		D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = srvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart();

		//メモリを番号分進ませる
		handleCPU.ptr += (srvDescriptorSize_ * index);

		return handleCPU;
	}

	///=====================================================/// 
	/// GPUデスクリプターを取得
	///=====================================================///
	D3D12_GPU_DESCRIPTOR_HANDLE SrvManager::GetGPUDescriptorHandle(uint32_t index) {

		//デスクリプタの最初のメモリを取得
		D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = srvDescriptorHeap_->GetGPUDescriptorHandleForHeapStart();

		//メモリを番号分進ませる
		handleGPU.ptr += (srvDescriptorSize_ * index);

		return handleGPU;
	}

	///=====================================================/// 
	/// デスクリプタテーブルの設定
	///=====================================================///
	void SrvManager::SetGraphicsRootDescriptorTable(UINT RootParameterIndex, uint32_t srvIndex) {

		dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(RootParameterIndex, GetGPUDescriptorHandle(srvIndex));
	}
}