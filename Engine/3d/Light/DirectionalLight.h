#pragma once

#include "Math/Vector3.h"
#include "Math/Vector4.h"

#include "d3dx12.h"
#include "wrl.h"

class DirectXCommon;

///=====================================================/// 
/// 平行光源ライト
///=====================================================///
class DirectionalLight {

	///-------------------------------------------/// 
	/// 構造体
	///-------------------------------------------///
private:

	//光源データ
	struct LightData {
		Vector4 color;     //色
		Vector3 direction; //向き
		float intensity;   //照度
	};

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// データをGPUに送信
	/// </summary>
	void SendDataForGPU();

	/// <summary>
	/// ImGuiの表示
	/// </summary>
	void DisplayImGui();

	///-------------------------------------------/// 
	/// ゲッター・セッター
	///-------------------------------------------///
public:

	void SetColor(const Vector4& color) { lightData_->color = color; }

	void SetDirection(const Vector3& direction) { lightData_->direction = direction; }

	void SetIntensity(float intensity) { lightData_->intensity = intensity; }

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
};