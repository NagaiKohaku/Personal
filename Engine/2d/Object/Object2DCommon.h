#pragma once

#include "Base/DirectXCommon.h"

#include "d3dx12.h"
#include "d3d12.h"

#include "vector"
#include "wrl.h"

/// <summary>
/// 2Dオブジェクト描画用の共通機能を提供するクラスです。
/// </summary>
/// <remarks>
/// - DirectX 12 を使用した描画共通設定の管理
/// - ルートシグネチャ、グラフィックパイプラインステート（PSO）の生成
/// - 複数ブレンドモードのサポート
/// - すべての 2D オブジェクトはこのクラスを通じて描画設定を適用します
/// </remarks>
class Object2DCommon {

	///-------------------------------------------/// 
	/// 列挙型
	///-------------------------------------------///
public:

	//ブレンドモードの種類
	enum BlendType {
		Normal,   //通常
		Add,      //加算
		Subtruct, //減算
		Multily,  //乗算
		Screen    //スクリーン
	};

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// Object2DCommonのシングルトンインスタンスを取得します。
	/// </summary>
	/// <remarks>
	/// 返り値に静的インスタンスを返します。
	/// </remarks>
	static Object2DCommon* GetInstance();

	/// <summary>
	/// Object2DCommonの初期化を行います。
	/// </summary>
	/// <remarks>
	/// - DirectXCommonのインスタンスを取得
	/// - グラフィックパイプラインの生成
	/// - ブレンドモードをノーマルに初期化
	/// </remarks>
	void Initialize();

	/// <summary>
	/// 2Dオブジェクトの描画に必要な共通設定を行います。
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
	/// 2Dオブジェクト描画用のルートシグネチャを作成します。
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
	/// 2Dオブジェクト描画用のグラフィックパイプラインステートを作成します。
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

	//ブレンドモード
	BlendType blendMode_;

	//ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;

	//グラフィックパイプラインステート
	std::vector<Microsoft::WRL::ComPtr<ID3D12PipelineState>> graphicsPipelineState_;

	///-------------------------------------------/// 
	/// セッター・ゲッター
	///-------------------------------------------///
public:

	/// <summary>
	/// DirectXCommonのインスタンスへのポインタを取得します。
	/// </summary>
	/// <returns> DirectXCommonのインスタンスへのポインタ </returns>
	DirectXCommon* GetDxCommon() const { return dxCommon_; }

	/// <summary>
	/// 描画時のブレンドモードを設定します。
	/// </summary>
	/// <param name="blendType"> 設定するブレンドモード </param>
	void SetBlendMode(BlendType blendType) { blendMode_ = blendType; }

};