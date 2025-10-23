#pragma once

#include "Base/DirectXCommon.h"

#include "Math/Vector3.h"
#include "Math/Vector4.h"

#include "d3dx12.h"
#include "wrl.h"

/// <summary>
/// 平行光源（Directional Light）を管理するクラスです。
/// </summary>
/// <remarks>
/// - DirectX 共通基底を使用して GPU バッファリソースを作成します。
/// - 光源データ（色、方向、照度）を LightData 構造体で保持します。
/// </remarks>
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
	/// ディレクショナルライトを初期化します。
	/// </summary>
	/// <remarks>
	/// - DirectXの共通基底インスタンスを取得
	/// - GPU用のバッファリソースを生成し、光源データをマッピングします。
	/// - 光源の色、向き、照度を初期設定
	/// </remarks>
	void Initialize();

	/// <summary>
	/// ディレクショナルライトの更新処理を行います。
	/// </summary>
	/// <remarks>
	/// - ライトの向きを正規化して、常に単位ベクトルに保ちます。
	/// </remarks>
	void Update();

	/// <summary>
	/// GPUにディレクショナルライトのデータを送信します。
	/// </summary>
	/// <remarks>
	/// - コマンドリストのルートパラメータ4にライトデータを設定します。
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
	/// 色の設定
	/// </summary>
	/// <param name="color">色</param>
	void SetColor(const Vector4& color) { lightData_->color = color; }

	/// <summary>
	/// 方向の設定
	/// </summary>
	/// <param name="direction">方向</param>
	void SetDirection(const Vector3& direction) { lightData_->direction = direction; }

	/// <summary>
	/// 照度の設定
	/// </summary>
	/// <param name="intensity">照度</param>
	void SetIntensity(float intensity) { lightData_->intensity = intensity; }

};