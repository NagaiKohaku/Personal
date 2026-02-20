#pragma once

#include "Base/DirectXCommon.h"

#include "d3dx12.h"
#include "d3d12.h"

#include "vector"
#include "wrl.h"

namespace MyEngine {

	/// <summary>
	/// 2Dオブジェクト描画用の共通機能を提供するクラスです。
	/// </summary>
	class Object2DCommon {

		///-------------------------------------------/// 
		/// 列挙型
		///-------------------------------------------///
	public:

		//ブレンドモードの種類
		enum class BlendType2D {
			NORMAL,   //通常
			ADD,      //加算
			SUBTRUCT, //減算
			MUTILY,  //乗算
			SCREEN    //スクリーン
		};

		///-------------------------------------------/// 
		/// メンバ関数
		///-------------------------------------------///
	public:

		/// <summary>
		/// Object2DCommonのシングルトンインスタンスを取得します。
		/// </summary>
		static Object2DCommon* GetInstance();

		/// <summary>
		/// Object2DCommonの初期化を行います。
		/// </summary>
		void Initialize(DirectXCommon* dxCommonPtr);

		/// <summary>
		/// 2Dオブジェクトの描画に必要な共通設定を行います。
		/// </summary>
		void CommonDrawSetting();

		///-------------------------------------------/// 
		/// クラス内処理関数
		///-------------------------------------------///
	private:

		/// <summary>
		/// 2Dオブジェクト描画用のルートシグネチャを作成します。
		/// </summary>
		void CreateRootSignature();

		/// <summary>
		/// 2Dオブジェクト描画用のグラフィックパイプラインステートを作成します。
		/// </summary>
		void CreateGraphicsPipeline();

		///-------------------------------------------/// 
		/// メンバ変数
		///-------------------------------------------///
	private:

		//DirectX基底
		DirectXCommon* dxCommon_ = nullptr;

		//ブレンドモード
		BlendType2D blendMode_;

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
		void SetBlendMode(BlendType2D blendType) { blendMode_ = blendType; }

	};
}