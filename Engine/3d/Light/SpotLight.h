#pragma once

#include "Base/DirectXCommon.h"

#include <Math/Vector/Vector3.h>
#include <Math/Vector/Vector4.h>

#include "d3dx12.h"
#include "wrl.h"

class DirectXCommon;

/// <summary>
/// スポットライト（Spot Light）を管理するクラスです。
/// </summary>
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
	/// スポットライトを初期化します。
	/// </summary>
	void Initialize();

	/// <summary>
	/// スポットライトを毎フレーム更新します。
	/// </summary>
	void Update();

	/// <summary>
	/// GPUにスポットライトのデータを送信します。
	/// </summary>
	/// <remarks>
	/// - コマンドリストのルートパラメータ6にライトデータを設定します。
	/// </remarks>
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
	MyEngine::DirectXCommon* dxCommon_ = nullptr;

	//バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> lightResource_ = nullptr;

	//光源データ
	LightData* lightData_ = nullptr;

	///-------------------------------------------/// 
	/// ゲッター・セッター
	///-------------------------------------------///
public:

	/// <summary>
	/// 色の設定
	/// </summary>
	/// <param name="color">色</param>
	void SetColor(const Vector4& color) { lightData_->color = color; }

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(const Vector3& position) { lightData_->position = position; }

	/// <summary>
	/// 照度の設定
	/// </summary>
	/// <param name="intensity">照度</param>
	void SetIntensity(float intensity) { lightData_->intensity = intensity; }

	/// <summary>
	/// 方向の設定
	/// </summary>
	/// <param name="direction">方向</param>
	void SetDirection(const Vector3& direction) { lightData_->direction = direction; }

	/// <summary>
	/// 距離の設定
	/// </summary>
	/// <param name="distance">距離</param>
	void SetDistance(float distance) { lightData_->distance = distance; }

	/// <summary>
	/// 減衰率の設定
	/// </summary>
	/// <param name="decay">減衰率</param>
	void SetDecay(float decay) { lightData_->decay = decay; }

	/// <summary>
	/// 拡散角度の設定
	/// </summary>
	/// <param name="angle">拡散角度</param>
	void SetAngle(float angle) { lightData_->diffusionAngle = angle; }

	/// <summary>
	/// 拡散率の設定
	/// </summary>
	/// <param name="falloffStart">拡散率</param>
	void SetFalloffStart(float falloffStart) { lightData_->diffusionRate = falloffStart; }
};