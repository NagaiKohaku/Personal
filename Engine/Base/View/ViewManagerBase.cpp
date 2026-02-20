#include "ViewManagerBase.h"

#include <cassert>

namespace MyEngine {

	///=====================================================/// 
	/// SRV番号を割り当て
	///=====================================================///
	uint32_t ViewManagerBase::Allocate() {

		//SRV番号が最大数を越えていないかの確認
		assert(GetMaxDescriptorCount() > currentIndex_);

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
	bool ViewManagerBase::AllocateCheck() {

		//SRV番号が最大数を越えていないかの確認
		if (GetMaxDescriptorCount() > currentIndex_) {
			return true;
		}

		return false;
	}

	///=====================================================/// 
	/// 解放されたメモリ番号を記録
	///=====================================================///
	void ViewManagerBase::RecordFreeIndex(uint32_t index) {

		//解放されたメモリ番号を記録
		freeIndices_.push(index);
	}

	///=====================================================/// 
	/// CPUデスクリプタハンドルを取得
	///=====================================================///
	D3D12_CPU_DESCRIPTOR_HANDLE ViewManagerBase::GetCPUDescriptorHandle(uint32_t index) {

		//デスクリプタの最初のメモリを取得
		D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap_->GetCPUDescriptorHandleForHeapStart();

		//メモリを番号分進ませる
		handleCPU.ptr += (descriptorSize_ * index);

		return handleCPU;
	}

	///=====================================================/// 
	/// GPUデスクリプタハンドルを取得
	///=====================================================///
	D3D12_GPU_DESCRIPTOR_HANDLE ViewManagerBase::GetGPUDescriptorHandle(uint32_t index) {

		//デスクリプタの最初のメモリを取得
		D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap_->GetGPUDescriptorHandleForHeapStart();

		//メモリを番号分進ませる
		handleGPU.ptr += (descriptorSize_ * index);

		return handleGPU;
	}
}