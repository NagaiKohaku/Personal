#pragma once
#include "d3dx12.h"
#include "d3d12.h"

#include "wrl.h"
#include "vector"

#include "Math/Vector3.h"

/// === 前方宣言 === ///

class DirectXCommon;

class Camera;

/// <summary>
/// デバッグオブジェクト描画用の共通機能を提供するクラスです。
/// </summary>
/// <remarks>
/// - DirectX 12 を使用した描画共通設定の管理
/// - ルートシグネチャ、グラフィックパイプラインステート（PSO）の生成
/// - 複数ブレンドモードのサポート
/// - すべてのデバッグオブジェクトはこのクラスを通じて描画設定を適用します
/// </remarks>
class DebugObjectCommon {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// DebugObjectCommonのシングルトンインスタンスを取得します。
	/// </summary>
	/// <remarks>
	/// 返り値に静的インスタンスを返します。
	/// </remarks>
	static DebugObjectCommon* GetInstance();

	/// <summary>
	/// DebugObjectCommonの初期化を行います。
	/// </summary>
	/// <remarks>
	/// - DirectXCommonのインスタンスを取得
	/// - グラフィックパイプラインの生成
	/// </remarks>
	void Initialize();

	/// <summary>
	/// デバッグオブジェクトの描画に必要な共通設定を行います。
	/// </summary>
	/// <remarks>
	/// - RootSignatureの設定
	/// - 現在のブレンドモードに対応するPSOの設定
	/// - メッシュの描画トポロジーの設定
	/// </remarks>
	void CommonDrawSetting();

	///-------------------------------------------/// 
	/// クラス内処理関数
	///-------------------------------------------///
private:

	/// <summary>
	/// デバッグオブジェクト描画用のルートシグネチャを作成します。
	/// </summary>
	/// <remarks>
	/// この関数では以下の設定を行います:
	/// - RootParameterの設定
	/// - DescriptorRangeでSRVの指定
	/// - PixelShader用のStaticSamplerを設定
	/// - RootSignatureの生成後、エラーがあればログ出力とアサートで停止
	/// </remarks>
	void CreateRootSignature();

	/// <summary>
	/// デバッグオブジェクト描画用のグラフィックパイプラインステートを作成します。
	/// </summary>
	/// <remarks>
	/// この関数では以下の処理を行います:
	/// - RootSignatureの生成
	/// - InputLayoutの設定
	/// - BlendStateの設定
	/// - RasterizerStateの設定
	/// - VertexShader/PixelShaderのコンパイル
	/// - DepthStencilStateの設定
	/// - 複数のブレンドモード用のPSOを生成し配列に格納
	/// </remarks>
	void CreateGraphicsPipeline();

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//DirectX基底
	DirectXCommon* dxCommon_ = nullptr;

	//カメラ
	Camera* camera_ = nullptr;

	//ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;

	//グラフィックパイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_ = nullptr;

	///-------------------------------------------/// 
	/// セッター・ゲッター
	///-------------------------------------------///
public:

	/// <summary>
	/// DirectX基底を取得
	/// </summary>
	/// <returns>DirectX基底</returns>
	DirectXCommon* GetDxCommon() const { return dxCommon_; }

	/// <summary>
	/// カメラを取得
	/// </summary>
	/// <returns>カメラ</returns>
	Camera* GetCamera() const { return camera_; }

	/// <summary>
	/// カメラの設定
	/// </summary>
	/// <param name="ptr">カメラ</param>
	void SetDefaultCamera(Camera* ptr) { camera_ = ptr; }

};