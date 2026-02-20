#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>
#include <dxcapi.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxcompiler.lib")

#include <cstdint>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include <wrl.h>

namespace MyEngine {

	/// === 前方宣言 === ///
	class WinApp;

	class RTVManager;

	class DSVManager;

	///-------------------------------------------/// 
	/// メモリリークチェッカー
	///-------------------------------------------///
	struct D3DResourceLeakChecker {
		~D3DResourceLeakChecker() {

#ifdef _DEBUG

			//リソースリークチェック
			Microsoft::WRL::ComPtr<IDXGIDebug1> debug;

			if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug)))) {

				debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);

				debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);

				debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
			}

#endif
		}
	};

	/// <summary>
	/// DirectX12 の基本描画管理クラスです。
	/// </summary>
	class DirectXCommon {

		///-------------------------------------------/// 
		/// メンバ関数
		///-------------------------------------------///
	public:

		/// <summary>
		/// DirectX12 の動作に必要な基本コンポーネントを初期化します。
		/// </summary>
		void Initialize(WinApp* winAppPtr);

		/// <summary>
		/// DirectX12 による描画処理を行うための各種レンダリング関連リソースを初期化します。
		/// </summary>
		void InitializeRendering(RTVManager* rtvPtr, DSVManager* dsvPtr);

		/// <summary>
		/// 次の描画に向けてコマンドリストを準備し、バックバッファを描画可能な状態にします。
		/// </summary>
		void PreDraw();

		/// <summary>
		/// 描画処理の終了処理を行い、バックバッファを画面に表示します。
		/// </summary>
		void PostDraw();

		/// <summary>
		/// 指定されたHLSLファイルを読み込み、指定のシェーダープロファイルでコンパイルします。
		/// </summary>
		/// <param name="filePath">コンパイル対象のHLSLファイルパス</param>
		/// <param name="profile">使用するシェーダープロファイル</param>
		/// <returns>コンパイル後のシェーダーバイナリ</returns>
		Microsoft::WRL::ComPtr<IDxcBlob> CompileShader(
			const std::wstring& filePath,
			const wchar_t* profile
		);

		/// <summary>
		/// 指定サイズのバッファ用リソースを生成します。
		/// </summary>
		/// <param name="sizeInBytes">バッファのサイズ（バイト単位）</param>
		/// <returns>生成されたバッファリソース</returns>
		Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);

		/// <summary>
		/// 指定された種類と数のデスクリプタヒープを生成します。
		/// </summary>
		/// <param name="heapType">生成するヒープの種類（CBV/SRV/UAVやRTV/DSVなど）</param>
		/// <param name="numDescriptors">ヒープに含めるデスクリプタの数</param>
		/// <param name="shaderVisible">シェーダーから参照可能にするかどうか</param>
		/// <returns>生成されたデスクリプタヒープ</returns>
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(
			D3D12_DESCRIPTOR_HEAP_TYPE heapType,
			UINT numDescriptors,
			bool shaderVisible
		);

		///-------------------------------------------/// 
		/// クラス内処理関数
		///-------------------------------------------///
	private:

		/// <summary>
		/// DirectX12 のデバイスおよび関連コンポーネントを初期化します。
		/// </summary>
		void InitializeDevice();

		/// <summary>
		/// コマンドキュー、コマンドアロケータ、コマンドリストを初期化します。
		/// </summary>
		void InitializeCommand();

		/// <summary>
		/// スワップチェーンを初期化し、画面に描画するためのバックバッファを構築します。
		/// </summary>
		void InitializeSwapChain();

		/// <summary>
		/// 画面描画で使用する深度バッファとステンシルバッファを生成し、初期化します。
		/// </summary>
		void InitializeDepthBuffer();

		/// <summary>
		/// バックバッファに描画するためのレンダーターゲットビューを初期化します。
		/// </summary>
		void InitializeRenderTargetView();

		/// <summary>
		/// 深度ステンシルバッファ用のビューを初期化します。
		/// </summary>
		void InitializeDepthStencilView();

		/// <summary>
		/// GPUとCPU間で処理の同期を行うフェンスを初期化します。
		/// </summary>
		void InitializeFence();

		/// <summary>
		/// 描画時のビューポート（表示領域）を初期化します。
		/// </summary>
		void InitializeViewportRect();

		/// <summary>
		/// 描画領域を限定するシザー矩形を初期化します。
		/// </summary>
		void InitializeScissorRect();

		/// <summary>
		/// DirectX Shader Compiler（DXC）関連のコンパイル環境を初期化します。
		/// </summary>
		void InitializeDXCCompile();

		/// <summary>
		/// フレームレート固定用の基準時間を初期化します。
		/// </summary>
		void InitializeFixFPS();

		/// <summary>
		/// 1フレームあたりの処理時間を制御して、描画を60FPSに固定します。
		/// </summary>
		void UpdateFixFPS();

		///-------------------------------------------/// 
		/// メンバ変数
		///-------------------------------------------///
	private:

		//WinAppクラス(借り物)
		WinApp* winApp_ = nullptr;

		//RTVマネージャー
		RTVManager* rtvManager_ = nullptr;

		//DSVマネージャー
		DSVManager* dsvManager_ = nullptr;

		//メモリリークチェック
		D3DResourceLeakChecker leakCheck;

		//デバイス
		Microsoft::WRL::ComPtr<ID3D12Device> device_ = nullptr;

		//DXGIファクトリー
		Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory_ = nullptr;

		//コマンドキュー
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_ = nullptr;

		//コマンドアロケータ
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator_ = nullptr;

		//コマンドリスト
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_ = nullptr;

		//スワップチェーン
		Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain_ = nullptr;

		//深度ステンシル
		Microsoft::WRL::ComPtr<ID3D12Resource> depthStancilResource_ = nullptr;

		//バックバッファー
		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffers_;

		//オフスクリーン用のリソース
		Microsoft::WRL::ComPtr<ID3D12Resource> offScreenResrouce_;

		//RTVハンドル
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_[2];

		//RTVのメモリ番号
		uint32_t rtvIndex_[2];

		//DSVハンドル
		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle_;

		//DSVのメモリ番号
		uint32_t dsvIndex_ = 0;

		//フェンス
		Microsoft::WRL::ComPtr<ID3D12Fence> fence_ = nullptr;

		//フェンスの値
		uint64_t fenceValue_ = 0;

		//ビューポート
		D3D12_VIEWPORT viewport_{};

		//シザー矩形
		D3D12_RECT scissorRect_{};

		//dxcUtils
		IDxcUtils* dxcUtils_ = nullptr;

		//dxcCompiler
		IDxcCompiler3* dxcCompiler_ = nullptr;

		//includeHander
		IDxcIncludeHandler* includeHandler_ = nullptr;

		//記録時間(FPS固定用)
		std::chrono::steady_clock::time_point reference_;

		///-------------------------------------------/// 
		/// ゲッター・セッター
		///-------------------------------------------///
	public:

		/// <summary>
		/// デバイスを取得
		/// </summary>
		/// <returns>デバイス</returns>
		ID3D12Device* GetDevice() { return device_.Get(); }

		/// <summary>
		/// コマンドリストを取得
		/// </summary>
		/// <returns>コマンドリスト</returns>
		ID3D12GraphicsCommandList* GetCommandList() { return commandList_.Get(); }

		/// <summary>
		/// バックバッファを取得
		/// </summary>
		/// <returns>バックバッファ</returns>
		size_t GetBackBufferCount() const { return backBuffers_.size(); }

		/// <summary>
		/// ビューポート行列を取得
		/// </summary>
		/// <returns>ビューポート行列</returns>
		D3D12_VIEWPORT GetViewport() const { return viewport_; }

		/// <summary>
		/// シザリング矩形を取得
		/// </summary>
		/// <returns>シザリング矩形</returns>
		D3D12_RECT GetScissorRect() const { return scissorRect_; }

	};
}