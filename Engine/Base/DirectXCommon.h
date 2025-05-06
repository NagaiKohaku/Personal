#pragma once
#include "Windows.h"
#include "d3d12.h"
#include "dxgi1_6.h"
#include "dxgidebug.h"
#include "dxcapi.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxcompiler.lib")

#include "cstdint"
#include "string"
#include "chrono"
#include "thread"
#include "vector"
#include "wrl.h"

#include "Math/Vector4.h"

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

			debug->Release();
		}

#endif
	}
};

///=====================================================/// 
/// DirectX基底
///=====================================================///
class DirectXCommon {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// シングルトンインスタンス
	/// </summary>
	/// <returns>インスタンス</returns>
	static DirectXCommon* GetInstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 描画系の初期化処理
	/// </summary>
	void InitializeRendering();

	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDraw();

	/// <summary>
	/// 描画後処理
	/// </summary>
	void PostDraw();

	/// <summary>
	/// シェーダーファイルのコンパイル
	/// </summary>
	/// <param name="modelName">ファイルパス</param>
	/// <param name="profile">ファイルの種類</param>
	/// <returns>バイナリ</returns>
	Microsoft::WRL::ComPtr<IDxcBlob> CompileShader(
		const std::wstring& filePath,
		const wchar_t* profile
	);

	/// <summary>
	/// ResourceObjectの生成
	/// </summary>
	/// <param name="sizeInBytes">サイズ</param>
	/// <returns>ResourceObject</returns>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);

	/// <summary>
	/// DescriptorHeapの生成
	/// </summary>
	/// <param name="heapType">ヒープの種類</param>
	/// <param name="numDescriptors">ヒープの数</param>
	/// <param name="shaderVisible">シェーダーを使用しているか</param>
	/// <returns></returns>
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(
		D3D12_DESCRIPTOR_HEAP_TYPE heapType,
		UINT numDescriptors,
		bool shaderVisible
	);

	void ClearDepthBuffer();

	///-------------------------------------------/// 
	/// ゲッター・セッター
	///-------------------------------------------///
public:

	/// <summary>
	/// デバイスのゲッター
	/// </summary>
	/// <returns>デバイス</returns>
	ID3D12Device* GetDevice() { return device_.Get(); }

	/// <summary>
	/// コマンドリストのゲッター
	/// </summary>
	/// <returns>コマンドリスト</returns>
	ID3D12GraphicsCommandList* GetCommandList() { return commandList_.Get(); }

	/// <summary>
	/// バックバッファのゲッター
	/// </summary>
	/// <returns>バックバッファ</returns>
	size_t GetBackBufferCount() const { return backBuffers_.size(); }

	D3D12_VIEWPORT GetViewport() const { return viewport_; }

	D3D12_RECT GetScissorRect() const { return scissorRect_; }

	///-------------------------------------------/// 
	/// クラス内処理関数
	///-------------------------------------------///
private:

	//デバイスの初期化
	void InitializeDevice();

	//コマンド関連の初期化
	void InitializeCommand();

	//スワップチェーンの初期化
	void InitializeSwapChain();

	//深度バッファの初期化
	void InitializeDepthBuffer();

	//レンダーターゲットビューの初期化
	void InitializeRenderTargetView();

	//深度ステンシルビューの初期化
	void InitializeDepthStencilView();

	//フェンスの初期化
	void InitializeFence();

	//ビューポート矩形の初期化
	void InitializeViewportRect();

	//シザリング矩形の初期化
	void InitializeScissorRect();

	//DXCコンパイラの初期化
	void InitializeDXCCompile();

	//FPS固定初期化
	void InitializeFixFPS();

	//FPS固定更新
	void UpdateFixFPS();

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//WinAppクラス(借り物)
	WinApp* winApp = nullptr;

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
};