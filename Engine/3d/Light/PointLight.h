#pragma once

#include "Math/Vector3.h"
#include "Math/Vector4.h"

#include "d3dx12.h"
#include "wrl.h"

class DirectXCommon;

///=====================================================/// 
/// 点光源ライト
///=====================================================///
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

	void SetPosition(const Vector3& position) { lightData_->position = position; }

	void SetColor(const Vector4& color) { lightData_->color = color; }

	void SetIntensity(float intensity) { lightData_->intensity = intensity; }

	void SetRadius(float radius) { lightData_->radius = radius; }

	void SetDecay(float decay) { lightData_->decay = decay; }

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