#pragma once
#include "d3dx12.h"
#include "d3d12.h"

#include "wrl.h"
#include "vector"

#include "Math/Vector3.h"

/// === 前方宣言 === ///

class DirectXCommon;

class Camera;

///=====================================================/// 
/// デバッグオブジェクト基底クラス
///=====================================================///
class DebugObjectCommon {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// シングルトンインスタンスを取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static DebugObjectCommon* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画前処理
	/// </summary>
	void CommonDrawSetting();

	///-------------------------------------------/// 
	/// クラス内処理関数
	///-------------------------------------------///
private:

	/// <summary>
	/// ルートシグネチャの生成
	/// </summary>
	void CreateRootSignature();

	/// <summary>
	/// グラフィックパイプラインの生成
	/// </summary>
	void CreateGraphicsPipeline();

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//DirectX基底
	DirectXCommon* dxCommon_ = nullptr;

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