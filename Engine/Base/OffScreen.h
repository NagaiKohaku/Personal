#pragma once

#include "d3d12.h"

#include "Math/Vector4.h"
#include "Math/Matrix4x4.h"

#include "wrl.h"
#include "vector"
#include "cstdint"
#include "string"

/// === 前方宣言 === ///

class DirectXCommon;

class RTVManager;

class DSVManager;

class SrvManager;

class Camera;

/// <summary>
/// オフスクリーン描画用のレンダーテクスチャや深度バッファを管理し、描画結果をスワップチェーンに転送できるクラスです。
/// </summary>
/// <remarks>
/// - シングルトンパターンで実装され、アプリケーション全体で 1 つのインスタンスを共有します。  
/// - 初期化時にレンダーテクスチャ、深度テクスチャ、マテリアル用バッファを生成し、描画パイプラインやビューを設定します。  
/// - PreDraw() で描画準備を行い、PostDraw() で描画結果をシェーダーで使用可能な状態に戻します。  
/// - DrawToSwapChain() によりオフスクリーンで描画した内容をスワップチェーンに転送して画面に表示できます。  
/// - ルートシグネチャとグラフィックスパイプラインを生成し、複数のブレンドモードや描画設定に対応します。  
/// - RenderTargetView、DepthStencilView、シェーダーリソースビューを生成し、リソース管理を統括します。  
/// - マテリアル情報として投影逆行列や色反転係数を保持し、描画時に利用できます。  
/// - 描画範囲の設定やクリア処理も行い、オフスクリーン描画を安定して行える設計です。  
/// - カメラやマテリアル設定の変更も可能で、柔軟な描画制御が可能です。  
/// </remarks>
class OffScreen {

	///-------------------------------------------/// 
	/// メンバ構造体
	///-------------------------------------------///
private:

	//マテリアル
	struct Material {
		Matrix4x4 projectionInverse;
		float colorReverseRatio;
	};

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// OffScreenのシングルトンインスタンスを取得します。
	/// </summary>
	/// <remarks>
	/// 返り値に静的インスタンスを返します。
	/// </remarks>
	static OffScreen* GetInstance();

	/// <summary>
	/// オフスクリーン描画用のリソースやパイプラインを初期化します。
	/// </summary>
	/// <remarks>
	/// - DirectX共通管理、RTV/DSV/SRVマネージャーのインスタンスを取得します。  
	/// - レンダーテクスチャと深度テクスチャを生成します。  
	/// - マテリアル用のバッファを生成し、データを書き込み可能にマッピングします。  
	/// - 投影逆行列や色反転比率などマテリアル初期値を設定します。  
	/// - レンダーターゲットビュー(RTV)と深度ステンシルビュー(DSV)を生成します。  
	/// - レンダーテクスチャと深度テクスチャのシェーダーリソースビュー(SRV)を作成します。  
	/// - 描画用のパイプラインを生成し、オフスクリーン描画が行える状態にします。  
	/// </remarks>
	void Initialize();

	/// <summary>
	/// オフスクリーン描画用のレンダーターゲットと深度バッファの描画準備を行います。
	/// </summary>
	/// <remarks>
	/// - レンダーテクスチャと深度テクスチャの状態を描画可能な状態に遷移させるバリアを設定します。  
	/// - オフスクリーン用のRTVとDSVを描画先に設定します。  
	/// - 指定したクリアカラーでレンダーテクスチャを初期化します。  
	/// - 深度バッファを初期深度値でクリアします。  
	/// - 描画範囲としてViewportとScissor矩形を設定し、描画命令を受け付ける状態にします。  
	/// </remarks>
	void PreDraw();

	/// <summary>
	/// オフスクリーン描画後にレンダーターゲットと深度バッファをシェーダーで使用できる状態に戻します。
	/// </summary>
	/// <remarks>
	/// - レンダーテクスチャを描画状態からピクセルシェーダーで参照可能な状態に遷移させます。  
	/// - 深度テクスチャを書き込み状態からピクセルシェーダーで参照可能な状態に遷移させます。  
	/// - これにより、次の描画パスでオフスクリーンの結果をテクスチャとして利用できます。  
	/// </remarks>
	void PostDraw();

	/// <summary>
	/// オフスクリーンで描画した内容をスワップチェーンのバックバッファに転送して描画します。
	/// </summary>
	/// <remarks>
	/// - オフスクリーン描画用のルートシグネチャをコマンドリストに設定します。  
	/// - 使用するパイプラインステートオブジェクトを設定します。  
	/// - 三角形リストとして描画するようにプリミティブトポロジーを設定します。  
	/// - オフスクリーンのレンダーテクスチャと深度テクスチャをシェーダーで使用できるようSRVをルートに設定します。  
	/// - マテリアル情報を定数バッファとしてルートに設定します。  
	/// - 三角形1つ分の描画命令を発行し、オフスクリーンの内容をスワップチェーンに描画します。  
	/// </remarks>
	void DrawToSwapChain();

	/// <summary>
	/// ImGuiの表示
	/// </summary>
	void ImGui();

	///-------------------------------------------/// 
	/// クラス内処理関数
	///-------------------------------------------///
private:

	/// <summary>
	/// オフスクリーン描画用のルートシグネチャを作成します。
	/// </summary>
	/// <remarks>
	/// - ルートパラメータを作成し、テクスチャSRV、深度テクスチャSRV、マテリアルCBVを設定します。  
	/// - 静的サンプラーを2種類設定し、バイリニアとポイントフィルタをPixelShaderで使用可能にします。  
	/// - D3D12SerializeRootSignatureでルートシグネチャをシリアライズし、デバイスで作成します。  
	/// </remarks>
	void CreateRootSignature();

	/// <summary>
	/// オフスクリーン描画用のグラフィックスパイプラインを生成します。
	/// </summary>
	/// <remarks>
	/// - ルートシグネチャを生成し設定します。  
	/// - InputLayoutを設定し、頂点データなしで描画可能にします。  
	/// - BlendStateとRasterizerStateを設定し、色書き込み・カリング・塗りつぶし方法を指定します。  
	/// - VertexShaderとPixelShaderをコンパイルしてバイナリを取得します。  
	/// - DepthStencilStateを設定し、深度機能を有効/無効にします。  
	/// - 基本PSOを生成し、RenderTarget・DepthStencil・BlendState・RasterizerState・トポロジ・サンプル情報を指定します。  
	/// - BlendModeごとにPSOを生成し、Add・Subtract・Multily・Screenモードを配列に追加します。  
	/// - 生成後は各PSOをoffScreenGraphicsPipelineState_配列に格納します。  
	/// - 各PSO生成時にHRESULTを確認し、失敗時はassertで停止します。  
	/// </remarks>
	void CreatePipeline();

	/// <summary>
	/// オフスクリーン用のRenderTargetView(RTV)を生成します。
	/// </summary>
	/// <remarks>
	/// - RTV番号をRTVManagerから確保します。  
	/// - 確保したRTV番号からCPUデスクリプタハンドルを取得します。  
	/// - RTVManagerを使用してRenderTextureリソースに対してRTVを生成します。  
	/// </remarks>
	void CreateRenderTargetView();

	/// <summary>
	/// オフスクリーン用のDepthStencilView(DSV)を生成します。
	/// </summary>
	/// <remarks>
	/// - DSV番号をDSVManagerから確保します。  
	/// - 確保したDSV番号からCPUデスクリプタハンドルを取得します。  
	/// - DSVManagerを使用してDepthTextureリソースに対してDSVを生成します。  
	/// </remarks>
	void CreateDepthStencilView();

	/// <summary>
	/// オフスクリーン用のレンダーテクスチャSRVを生成します。
	/// </summary>
	/// <remarks>
	/// - SRV番号をSrvManagerから確保します。  
	/// - 確保したSRV番号からCPUデスクリプタハンドルを取得します。  
	/// - SrvManagerを使用してレンダーテクスチャリソースに対してSRVを生成します。  
	/// </remarks>
	void CreateRenderTextureSRV();

	/// <summary>
	/// 深度テクスチャ用のSRVを生成します。
	/// </summary>
	/// <remarks>
	/// - SRV番号をSrvManagerから確保します。  
	/// - 確保したSRV番号からCPUデスクリプタハンドルを取得します。  
	/// - SrvManagerを使用して深度テクスチャリソースに対してSRVを生成します。  
	/// </remarks>
	void CreateDepthTextureSRV();

	/// <summary>
	/// 指定サイズ・フォーマットのレンダーターゲット用テクスチャを生成します。
	/// </summary>
	/// <remarks>
	/// - 2Dテクスチャとしてリソース情報を設定します。  
	/// - VRAM上にDefaultHeapとして作成します。  
	/// - RenderTarget用のクリア値を設定します。  
	/// - CreateCommittedResourceでリソースを生成し、初期状態をPixelShaderResourceに設定します。  
	/// </remarks>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateRenderTexture(
		Microsoft::WRL::ComPtr<ID3D12Device> device,
		uint32_t width,
		uint32_t height,
		DXGI_FORMAT format,
		const Vector4& clearColor
	);

	/// <summary>
	/// 指定サイズ・フォーマットの深度ステンシル用テクスチャを生成します。
	/// </summary>
	/// <remarks>
	/// - 2Dテクスチャとしてリソース情報を設定します。  
	/// - VRAM上にDefaultHeapとして作成します。  
	/// - DepthStencil用のクリア値をDepth=1.0fに設定します。  
	/// - CreateCommittedResourceでリソースを生成し、初期状態をPixelShaderResourceに設定します。  
	/// </remarks>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateDepthTexture(
		Microsoft::WRL::ComPtr<ID3D12Device> device,
		uint32_t width,
		uint32_t height,
		DXGI_FORMAT format
	);

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//DirectX基底
	DirectXCommon* dxCommon_ = nullptr;

	//RTVマネージャー
	RTVManager* rtvManager_ = nullptr;

	//DSVマネージャー
	DSVManager* dsvManager_ = nullptr;

	//SRVマネージャー
	SrvManager* srvManager_ = nullptr;

	//カメラ
	Camera* camera_ = nullptr;

	//オフスクリーン用のテクスチャ
	Microsoft::WRL::ComPtr<ID3D12Resource> renderTextureResrouce_;

	//DepthTextreのリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> depthTextureResource_;

	//オフスクリーン用のマテリアルリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_;

	//RTVハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE offScreenRTVHandle_;

	//RTVのメモリ番号
	uint32_t rtvIndex_ = 0;

	//DSVハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE offScreenDSVHandle_;

	//DSVのメモリ番号
	uint32_t dsvIndex_ = 0;

	//renderTextureハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE renderTextureSRVHandle_;

	//renderTextureのメモリ番号
	uint32_t renderTextureSRVIndex_ = 0;

	//DepthTextureハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE depthTextureSRVHandle_;

	//DepthTextureのメモリ番号
	uint32_t depthTextureSRVIndex_ = 0;

	//オフスクリーン用のマテリアルデータ
	Material* materialData_ = nullptr;

	//ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> offScreenRootSignature_ = nullptr;

	//グラフィックパイプラインステート
	std::vector<Microsoft::WRL::ComPtr<ID3D12PipelineState>> offScreenGraphicsPipelineState_;

	//現在のシェーダー名
	std::wstring currentShaderName_;

	//オフスクリーンのクリア値
	Vector4 offScreenClearColor_ = { 0.0f,0.0f,0.0f,1.0f };

	///-------------------------------------------/// 
	/// ゲッター・セッター
	///-------------------------------------------///
public:

	/// <summary>
	/// デフォルトカメラの設定
	/// </summary>
	/// <param name="ptr">カメラポインタ</param>
	void SetDefaultCamera(Camera* ptr);

	/// <summary>
	/// 色反転係数の設定
	/// </summary>
	/// <param name="num">色反転係数</param>
	void SetColorReverseRatio(float num) { materialData_->colorReverseRatio = num; }

	/// <summary>
	/// 色反転係数の取得
	/// </summary>
	/// <returns>色反転係数</returns>
	float GetColorReverseRatio() { return materialData_->colorReverseRatio; }

};