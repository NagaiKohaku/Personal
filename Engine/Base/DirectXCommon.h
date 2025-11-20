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
		}

#endif
	}
};

/// <summary>
/// DirectX12 の基本描画管理クラスです。
/// </summary>
/// <remarks>
/// - シングルトンパターンで実装され、アプリケーション全体で 1 つのインスタンスを共有します。  
/// - WinApp からウィンドウ情報を取得し、描画に必要な DirectX12 デバイスやコマンド関連リソースを初期化します。  
/// - スワップチェーン、バックバッファ、深度ステンシルバッファ、RTV/DSV を管理し描画環境を整えます。  
/// - コマンドリストの準備、描画前後処理（PreDraw / PostDraw）により、フレームごとの描画フローを制御します。  
/// - HLSL ファイルのコンパイルをサポートし、シェーダーバイナリを取得可能です。  
/// - GPU バッファリソース、デスクリプタヒープの生成を行い、レンダリングで利用します。  
/// - フレームレート固定処理により、60FPS に安定した描画が可能です。  
/// - 内部で DXC コンパイラ、フェンス、ビューポート、シザー矩形などを管理し描画の正確性と同期を確保します。  
/// - ゲッターを通じてデバイス、コマンドリスト、バックバッファ、ビューポート、シザー矩形を取得できます。  
/// </remarks>
class DirectXCommon {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// DirectXCommonのシングルトンインスタンスを取得します。
	/// </summary>
	/// <remarks>
	/// 返り値に静的インスタンスを返します。
	/// </remarks>
	static DirectXCommon* GetInstance();

	/// <summary>
	/// DirectX12 の動作に必要な基本コンポーネントを初期化します。
	/// </summary>
	/// <remarks>
	/// - WinApp からウィンドウ情報を取得します。  
	/// - FPS を固定するための初期化を行います。  
	/// - DirectX12 デバイス（GPU）を生成し使用可能な状態にします。  
	/// - コマンドキュー・アロケータ・コマンドリストなど、描画に必要なコマンド関連の初期化を行います。  
	/// </remarks>
	void Initialize();

	/// <summary>
	/// DirectX12 による描画処理を行うための各種レンダリング関連リソースを初期化します。
	/// </summary>
	/// <remarks>
	/// - RTVManager / DSVManager を取得し、レンダーターゲットと深度ステンシルの管理体制を整えます。  
	/// - スワップチェーンを生成してフロント／バックバッファを扱えるようにします。  
	/// - 深度バッファを作成し、DSV を初期化します。  
	/// - RTV を初期化し、描画先の設定を行えるようにします。  
	/// - GPU 同期のためのフェンスを作成します。  
	/// - ビューポート矩形とシザリング矩形を設定します。  
	/// - HLSL をコンパイルするための DXC コンパイラを初期化します。  
	/// </remarks>
	void InitializeRendering();

	/// <summary>
	/// 次の描画に向けてコマンドリストを準備し、バックバッファを描画可能な状態にします。
	/// </summary>
	/// <remarks>
	/// - 現在のスワップチェーンのバックバッファを描画状態に遷移させます。  
	/// - 描画先としてバックバッファと深度ステンシルを設定します。  
	/// - 指定した色でバックバッファをクリアし、深度値もクリアします。  
	/// - ビューポートとシザー矩形を設定して描画範囲を決定します。  
	/// - この処理により、次の描画コマンドが正しく実行できる状態になります。  
	/// </remarks>
	void PreDraw();

	/// <summary>
	/// 描画処理の終了処理を行い、バックバッファを画面に表示します。
	/// </summary>
	/// <remarks>
	/// - 現在のバックバッファを描画状態からプレゼント状態に遷移させます。  
	/// - コマンドリストをクローズして内容を確定し、GPUに実行させます。  
	/// - スワップチェーンを通じて画面に描画結果を表示します。  
	/// - GPUが描画処理を完了するまでフェンスを用いて待機します。  
	/// - フレームレート固定用の時間管理処理を更新します。  
	/// - 次フレーム用にコマンドアロケータとコマンドリストをリセットして準備します。  
	/// </remarks>
	void PostDraw();

	/// <summary>
	/// 指定されたHLSLファイルを読み込み、指定のシェーダープロファイルでコンパイルします。
	/// </summary>
	/// <param name="filePath">コンパイル対象のHLSLファイルパス</param>
	/// <param name="profile">使用するシェーダープロファイル</param>
	/// <returns>コンパイル後のシェーダーバイナリ</returns>
	/// <remarks>
	/// - HLSLファイルをUTF-8形式で読み込みます。  
	/// - エントリーポイントを "main" に固定してコンパイルします。  
	/// - デバッグ情報の埋め込みや最適化オプションを設定します。  
	/// - コンパイルエラーや警告がある場合はログを出力してプログラムを停止します。  
	/// - 成功した場合、実行可能なバイナリ部分を取得して返します。  
	/// - 使用済みのリソースは解放されます。  
	/// </remarks>
	Microsoft::WRL::ComPtr<IDxcBlob> CompileShader(
		const std::wstring& filePath,
		const wchar_t* profile
	);

	/// <summary>
	/// 指定サイズのバッファ用リソースを生成します。
	/// </summary>
	/// <param name="sizeInBytes">バッファのサイズ（バイト単位）</param>
	/// <returns>生成されたバッファリソース</returns>
	/// <remarks>
	/// - GPUにアップロード可能なヒープを利用してバッファ用リソースを確保します。  
	/// - リソースはバッファ用として設定し、行優先レイアウトで作成します。  
	/// - 高さ、深さ、ミップレベル、サンプル数はバッファ用の標準値に設定します。  
	/// - リソース生成後、読み取り可能な状態に初期化します。  
	/// </remarks>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);

	/// <summary>
	/// 指定された種類と数のデスクリプタヒープを生成します。
	/// </summary>
	/// <param name="heapType">生成するヒープの種類（CBV/SRV/UAVやRTV/DSVなど）</param>
	/// <param name="numDescriptors">ヒープに含めるデスクリプタの数</param>
	/// <param name="shaderVisible">シェーダーから参照可能にするかどうか</param>
	/// <returns>生成されたデスクリプタヒープ</returns>
	/// <remarks>
	/// - ヒープの種類、デスクリプタ数、シェーダー可視フラグを設定して生成します。  
	/// - シェーダー可視にする場合は、フラグに SHADER_VISIBLE を設定します。  
	/// </remarks>
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
	/// <remarks>
	/// - デバッグビルド時に D3D12 デバッグレイヤーと GPU ベース検証を有効化します。  
	/// - グラフィックデバイス管理のため DXGI ファクトリを生成します。  
	/// - 高パフォーマンス順にアダプタを列挙し、物理 GPU を選択します。  
	/// - 対応している最高の D3D12 機能レベルでデバイス生成を試行します。  
	/// - デバッグビルド時にエラー・警告でブレークし不要メッセージを抑制します。  
	/// - デバイス初期化完了のログを出力します。  
	/// </remarks>
	void InitializeDevice();

	/// <summary>
	/// コマンドキュー、コマンドアロケータ、コマンドリストを初期化します。
	/// </summary>
	/// <remarks>
	/// - D3D12_COMMAND_QUEUE_DESC を用いてコマンドキューを生成します。  
	/// - DIRECT タイプのコマンドアロケータを生成します。  
	/// - コマンドアロケータを基にコマンドリストを生成します。  
	/// </remarks>
	void InitializeCommand();

	/// <summary>
	/// スワップチェーンを初期化し、画面に描画するためのバックバッファを構築します。
	/// </summary>
	/// <remarks>
	/// - クライアント領域サイズ・色情報・バッファ数などを設定します。  
	/// - ダブルバッファ・FLIP_DISCARD を利用した標準的なスワップチェーンを作成します。  
	/// - コマンドキューとウィンドウハンドルからスワップチェーンを生成します。  
	/// </remarks>
	void InitializeSwapChain();

	/// <summary>
	/// 画面描画で使用する深度バッファとステンシルバッファを生成し、初期化します。
	/// </summary>
	/// <remarks>
	/// - ウィンドウの幅と高さに合わせて深度バッファ用のメモリを確保します。  
	/// - 深度値を扱うためのテクスチャを作成し、ステンシル情報も含めて設定します。  
	/// - バッファをビデオメモリ上に作成し、書き込み可能な状態に初期化します。  
	/// - 初期化時に深度値を最大値でクリアする設定を行います。  
	/// </remarks>
	void InitializeDepthBuffer();

	/// <summary>
	/// バックバッファに描画するためのレンダーターゲットビューを初期化します。
	/// </summary>
	/// <remarks>
	/// - スワップチェーンから画面描画用の全てのバッファを取得します。  
	/// - 取得した各バッファを内部リストに登録します。  
	/// - バッファごとにレンダーターゲットビュー用のメモリを確保し、CPU側のハンドルを取得します。  
	/// - 各バッファに対してレンダーターゲットビューを生成します。  
	/// </remarks>
	void InitializeRenderTargetView();

	/// <summary>
	/// 深度ステンシルバッファ用のビューを初期化します。
	/// </summary>
	/// <remarks>
	/// - 深度ステンシル用のメモリを確保します。  
	/// - CPU側でアクセス可能なハンドルを取得します。  
	/// - 確保したメモリと深度バッファを関連付け、DSVを生成します。  
	/// </remarks>
	void InitializeDepthStencilView();

	/// <summary>
	/// GPUとCPU間で処理の同期を行うフェンスを初期化します。
	/// </summary>
	/// <remarks>
	/// - フェンスの初期値を設定します。  
	/// - デバイスを用いてフェンスを生成します。  
	/// </remarks>
	void InitializeFence();

	/// <summary>
	/// 描画時のビューポート（表示領域）を初期化します。
	/// </summary>
	/// <remarks>
	/// - ビューポートの幅と高さをウィンドウのクライアント領域サイズに合わせます。  
	/// - 左上座標を (0,0) に設定し、深度範囲を 0.0 から 1.0 に設定します。  
	/// - これにより画面全体に描画が行われるようになります。  
	/// </remarks>
	void InitializeViewportRect();

	/// <summary>
	/// 描画領域を限定するシザー矩形を初期化します。
	/// </summary>
	/// <remarks>
	/// - 描画対象となる矩形の左上と右下の座標をウィンドウのクライアント領域に合わせて設定します。  
	/// - これによりビューポートと同じ範囲で描画が制限されます。  
	/// </remarks>
	void InitializeScissorRect();

	/// <summary>
	/// DirectX Shader Compiler（DXC）関連のコンパイル環境を初期化します。
	/// </summary>
	/// <remarks>
	/// - DXCのユーティリティ（dxcUtils）を生成して初期化します。  
	/// - DXCのコンパイラオブジェクト（dxcCompiler）を生成して初期化します。  
	/// - シェーダーのインクルードを扱うための標準ハンドラ（includeHandler）を生成します。  
	/// </remarks>
	void InitializeDXCCompile();

	/// <summary>
	/// フレームレート固定用の基準時間を初期化します。
	/// </summary>
	/// <remarks>
	/// - 現在の時間を取得して内部の基準時間として記録します。  
	/// - この時間を元にフレームごとの処理速度を制御し、一定FPSでの描画を可能にします。  
	/// </remarks>
	void InitializeFixFPS();

	/// <summary>
	/// 1フレームあたりの処理時間を制御して、描画を60FPSに固定します。
	/// </summary>
	/// <remarks>
	/// - 前回のフレームからの経過時間を計測します。  
	/// - 1/60秒より短ければ、経過時間が1/60秒になるまで微小なスリープを繰り返します。  
	/// - 現在時間を基準時間として更新し、次のフレームの制御に利用します。  
	/// - この処理によりフレームレートを安定させ、描画速度の変動を防ぎます。  
	/// </remarks>
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