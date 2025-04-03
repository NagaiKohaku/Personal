#pragma once

#include "Math/Vector3.h"
#include "Math/Vector4.h"

#include "d3dx12.h"
#include "wrl.h"

class DirectXCommon;

///=====================================================/// 
/// スポットライト
///=====================================================///
class SpotLight {

	///-------------------------------------------/// 
	/// 構造体
	///-------------------------------------------///
private:

	//光源データ
	struct LightData {
		Vector4 color;        //色
		Vector3 position;     //位置
		float intensity;      //照度
		Vector3 direction;    //方向
		float distance;       //最大距離
		float decay;          //減衰率
		float diffusionAngle; //拡散角度
		float diffusionRate;  //拡散率
		float padding[2];     //穴埋め
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

	void SetPosition(const Vector3& position) { lightData_->position = position; }

	void SetIntensity(float intensity) { lightData_->intensity = intensity; }

	void SetDirection(const Vector3& direction) { lightData_->direction = direction; }

	void SetDistance(float distance) { lightData_->distance = distance; }

	void SetDecay(float decay) { lightData_->decay = decay; }

	void SetAngle(float angle) { lightData_->diffusionAngle = angle; }

	void SetFalloffStart(float falloffStart) { lightData_->diffusionRate = falloffStart; }

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