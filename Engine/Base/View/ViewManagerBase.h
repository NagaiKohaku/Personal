#pragma once
#include <d3d12.h>
#include <cstdint>
#include <wrl.h>
#include <queue>

namespace MyEngine {

	/// === 前方宣言 === ///
	class DirectXCommon;

	class ViewManagerBase {

	public:

		/// <summary>
		/// 初期化処理
		/// </summary>
		/// <param name="dxCommonPtr">DirectX基底のポインタ</param>
		virtual void Initialize(DirectXCommon* dxCommonPtr) = 0;

		/// <summary>
		/// 描画前処理
		/// </summary>
		virtual void PreDraw() = 0;

		/// <summary>
		/// デスクリプタ番号の最大数を取得
		/// </summary>
		/// <returns>最大数のデスクリプタ番号</returns>
		virtual uint32_t GetMaxDescriptorCount() = 0;

	public:

		/// <summary>
		/// 使用可能な番号を割り当てます。
		/// </summary>
		/// <returns>割り当てられた番号</returns>
		uint32_t Allocate();

		/// <summary>
		/// 使用可能な番号があるかを確認します。
		/// </summary>
		/// <returns>true: 割り当て可能 / false: 割り当て不可</returns>
		bool AllocateCheck();

		/// <summary>
		/// 解放されたメモリ番号を記録します。
		/// </summary>
		/// <param name="index">解放した番号</param>
		void RecordFreeIndex(uint32_t index);

	protected:

		//DirectX基底
		DirectXCommon* dxCommon_ = nullptr;

		//デスクリプタヒープ
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap_;

		//解放済みView番号
		std::queue<uint32_t> freeIndices_;

		//デスクリプタサイズ
		uint32_t descriptorSize_;

		//現在のView番号
		uint32_t currentIndex_ = 0;

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
	};
}