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
class Object3DCommon {

	///-------------------------------------------/// 
	/// 列挙型
	///-------------------------------------------///
public:

	//ブレンドモードの種類
	enum class BlendType3D {
		NORMAL,   //通常
		ADD,      //加算
		SUBTRUCT, //減算
		MULTILY,  //乗算
		SCREEN    //スクリーン
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
	static Object3DCommon* GetInstance();

	/// <summary>
	/// Object3DCommonの初期化を行います。
	/// </summary>
	void Initialize();

	/// <summary>
	/// Object3DCommonの更新処理を行います。
	/// </summary>
	void Update();

	/// <summary>
	/// 3Dオブジェクトの描画に必要な共通設定を行います。
	/// </summary>
	void CommonDrawSetting();

	///-------------------------------------------/// 
	/// クラス内処理関数
	///-------------------------------------------///
private:

	/// <summary>
	/// 3Dオブジェクト描画用のルートシグネチャを作成します。
	/// </summary>
	void CreateRootSignature();

	/// <summary>
	/// 3Dオブジェクト描画用のグラフィックパイプラインステートを作成します。
	/// </summary>
	void CreateGraphicsPipeline();

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//DirectX基底
	DirectXCommon* dxCommon_ = nullptr;

	//ブレンドモード
	BlendType3D blendMode_;

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
	void SetBlendMode(BlendType3D blendType) { blendMode_ = blendType; }

	/// <summary>
	/// キューブテクスチャのファイルパスの設定
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	void SetTextureCubeFilePath(const std::string& filePath) { textureCubeFilePath_ = filePath; }

};