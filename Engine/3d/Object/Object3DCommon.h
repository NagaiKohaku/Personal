#pragma once

#include "3d/Light/DirectionalLight.h"
#include "3d/Light/PointLight.h"
#include "3d/Light/SpotLight.h"

#include "Math/Vector3.h"

#include "d3dx12.h"
#include "d3d12.h"

#include "wrl.h"
#include "vector"
#include "string"

/// === 前方宣言 === //

class DirectXCommon;

class Camera;

/// <summary>
/// 2Dオブジェクト描画用の共通機能を提供するクラスです。
/// </summary>
/// <remarks>
/// - DirectX 12 を使用した描画共通設定の管理
/// - ルートシグネチャ、グラフィックパイプラインステート（PSO）の生成
/// - 複数ブレンドモードのサポート
/// - すべての 3D オブジェクトはこのクラスを通じて描画設定を適用します
/// </remarks>
class Object3DCommon {

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
	/// 構造体
	///-------------------------------------------///
private:

	struct CameraForGPU {
		Vector3 worldPosition;
	};

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// Object3DCommonのシングルトンインスタンスを取得します。
	/// </summary>
	/// <remarks>
	/// 返り値に静的インスタンスを返します。
	/// </remarks>
	static Object3DCommon* GetInstance();

	/// <summary>
	/// Object3DCommonの初期化を行います。
	/// </summary>
	/// <remarks>
	/// - DirectXCommonのインスタンスを取得
	/// - グラフィックパイプラインの生成
	/// - ライト系の初期設定
	/// - ブレンドモードをノーマルに初期化
	/// </remarks>
	void Initialize();

	/// <summary>
	/// Object3DCommonの更新処理を行います。
	/// </summary>
	/// <remarks>
	/// - カメラの位置を取得し、GPU用のカメラデータに反映
	/// - ライト系の更新処理
	/// </remarks>
	void Update();

	/// <summary>
	/// 3Dオブジェクトの描画に必要な共通設定を行います。
	/// </summary>
	/// <remarks>
	/// - RootSignatureの設定
	/// - 現在のブレンドモードに対応するPSOの設定
	/// - メッシュの描画トポロジーの設定
	/// - カメラ情報の設定
	/// - ライト情報の設定
	/// </remarks>
	void CommonDrawSetting();

	///-------------------------------------------/// 
	/// クラス内処理関数
	///-------------------------------------------///
private:

	/// <summary>
	/// 3Dオブジェクト描画用のルートシグネチャを作成します。
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
	/// 3Dオブジェクト描画用のグラフィックパイプラインステートを作成します。
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

	//デフォルトカメラ
	Camera* defaultCamera_ = nullptr;

	//平行光源ライト
	std::unique_ptr<DirectionalLight> directionalLight_;

	//点光源ライト
	std::unique_ptr<PointLight> pointLight_;

	//スポットライト
	std::unique_ptr<SpotLight> spotLight_;

	//バッファリソース内のデータを指すポインタ
	CameraForGPU* cameraForGpuData = nullptr;

	//バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> cameraForGpuResource = nullptr;

	//ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;

	//グラフィックパイプラインステート
	std::vector<Microsoft::WRL::ComPtr<ID3D12PipelineState>> graphicsPipelineState_;

	//キューブテクスチャのファイルパス
	std::string textureCubeFilePath_;

	///-------------------------------------------/// 
	/// セッター・ゲッター
	///-------------------------------------------///
public:

	/// <summary>
	/// DirectX基底を取得
	/// </summary>
	/// <returns></returns>
	DirectXCommon* GetDxCommon() const { return dxCommon_; }

	/// <summary>
	/// デフォルトカメラを取得
	/// </summary>
	/// <returns></returns>
	Camera* GetDefaultCamera() const { return defaultCamera_; }

	/// <summary>
	/// デフォルトカメラの設定
	/// </summary>
	/// <param name="camera">カメラ</param>
	void SetDefaultCamera(Camera* camera) { defaultCamera_ = camera; }

	/// <summary>
	/// ブレンドモードの設定
	/// </summary>
	/// <param name="blendType">ブレンドタイプ</param>
	void SetBlendMode(BlendType blendType) { blendMode_ = blendType; }

	/// <summary>
	/// キューブテクスチャのファイルパスの設定
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	void SetTextureCubeFilePath(const std::string& filePath) { textureCubeFilePath_ = filePath; }

};