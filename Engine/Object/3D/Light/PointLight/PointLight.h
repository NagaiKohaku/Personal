#pragma once

#include "Base/DirectXCommon.h"

#include <Math/Vector/Vector3.h>
#include <Math/Vector/Vector4.h>

#include "d3dx12.h"
#include "wrl.h"

namespace MyEngine {

	/// <summary>
	/// 点光源（Point Light）を管理するクラスです。
	/// </summary>
	class PointLight {

		///-------------------------------------------/// 
		/// 構造体
		///-------------------------------------------///
	private:

		//光源データ
		struct LightData {
			Vector4 color;    //色
			Vector3 position; //座標
			float intensity;  //照度
			float radius;     //半径
			float decay;      //減衰率
			float padding[2]; //穴埋め
		};

		///-------------------------------------------/// 
		/// メンバ関数
		///-------------------------------------------///
	public:

		/// <summary>
		/// ポイントライトの初期化を行います。
		/// </summary>
		void Initialize(DirectXCommon* dxCommonPtr);

		/// <summary>
		/// ポイントライトのデータを毎フレーム更新します。
		/// </summary>
		void Update();

		/// <summary>
		/// GPUにポイントライトのデータを送信します。
		/// </summary>
		void SendDataForGPU();

		/// <summary>
		/// ImGuiの表示
		/// </summary>
		void DisplayImGui();

		///-------------------------------------------/// 
		/// メンバ変数
		///-------------------------------------------///
	private:

		//DirectX基底
		DirectXCommon* dxCommon_ = nullptr;

		//バッファリソース
		Microsoft::WRL::ComPtr<ID3D12Resource> lightResource_ = nullptr;

		//光源データ
		LightData* lightData_ = nullptr;

		///-------------------------------------------/// 
		/// ゲッター・セッター
		///-------------------------------------------///
	public:

		/// <summary>
		/// 座標の設定
		/// </summary>
		/// <param name="position">座標</param>
		void SetPosition(const Vector3& position) { lightData_->position = position; }

		/// <summary>
		/// 色の設定
		/// </summary>
		/// <param name="color">色</param>
		void SetColor(const Vector4& color) { lightData_->color = color; }

		/// <summary>
		/// 照度の設定
		/// </summary>
		/// <param name="intensity">照度</param>
		void SetIntensity(float intensity) { lightData_->intensity = intensity; }

		/// <summary>
		/// 半径の設定
		/// </summary>
		/// <param name="radius">半径</param>
		void SetRadius(float radius) { lightData_->radius = radius; }

		/// <summary>
		/// 減衰率の設定
		/// </summary>
		/// <param name="decay">減衰率</param>
		void SetDecay(float decay) { lightData_->decay = decay; }
	};
}