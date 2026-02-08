#pragma once

#include <Base/DirectXCommon.h>
#include <3d/Camera/Camera.h>

#include "d3dx12.h"
#include "d3d12.h"

#include "wrl.h"
#include "vector"

#include <Math/Vector/Vector3.h>

/// <summary>
/// デバッグオブジェクト描画用の共通機能を提供するクラスです。
/// </summary>
class DebugObjectCommon {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// DebugObjectCommonのシングルトンインスタンスを取得します。
	/// </summary>
	static DebugObjectCommon* GetInstance();

	/// <summary>
	/// DebugObjectCommonの初期化を行います。
	/// </summary>
	void Initialize();

	/// <summary>
	/// デバッグオブジェクトの描画に必要な共通設定を行います。
	/// </summary>
	void CommonDrawSetting();

	///-------------------------------------------/// 
	/// クラス内処理関数
	///-------------------------------------------///
private:

	/// <summary>
	/// デバッグオブジェクト描画用のルートシグネチャを作成します。
	/// </summary>
	void CreateRootSignature();

	/// <summary>
	/// デバッグオブジェクト描画用のグラフィックパイプラインステートを作成します。
	/// </summary>
	void CreateGraphicsPipeline();

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//DirectX基底
	MyEngine::DirectXCommon* dxCommon_ = nullptr;

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
	MyEngine::DirectXCommon* GetDxCommon() const { return dxCommon_; }

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