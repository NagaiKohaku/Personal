#pragma once
#include "d3dx12.h"
#include "d3d12.h"

#include "wrl.h"
#include "vector"

namespace MyEngine {

	/// === 前方宣言 === ///

	class DirectXCommon;

	class Camera;

	/// <summary>
	/// スカイボックス描画用の共通機能を提供するクラスです。
	/// </summary>
	class SkyBoxCommon {

		///-------------------------------------------/// 
		/// メンバ関数
		///-------------------------------------------///
	public:

		/// <summary>
		/// SkyBoxCommonのシングルトンインスタンスを取得します。
		/// </summary>
		static SkyBoxCommon* GetInstance();

		/// <summary>
		/// SkyBoxCommonの初期化を行います。
		/// </summary>
		void Initialize(DirectXCommon* dxCommonPtr);

		/// <summary>
		/// 更新処理
		/// </summary>
		void Update();

		/// <summary>
		/// スカイボックスの描画に必要な共通設定を行います。
		/// </summary>
		void CommonDrawSetting();

		///-------------------------------------------/// 
		/// クラス内処理関数
		///-------------------------------------------///
	private:

		/// <summary>
		/// スカイボックス描画用のルートシグネチャを作成します。
		/// </summary>
		void CreateRootSignature();

		/// <summary>
		/// スカイボックス描画用のグラフィックパイプラインステートを作成します。
		/// </summary>
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
}