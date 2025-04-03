#pragma once

#include "Base/LayerType.h"

#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Matrix4x4.h"
#include "Math/WorldTransform.h"

#include "DirectXTex.h"
#include "d3d12.h"

#include "wrl.h"
#include "string"
#include "vector"
#include "memory"

class Object3DCommon;

class Model;

class Camera;

class DebugLine;

///=====================================================/// 
/// 3Dオブジェクト
///=====================================================///
class Object3D {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Object3D();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(LayerType layer);

	/// <summary>
	/// デバッグの描画処理
	/// </summary>
	void DebugDraw();

	/// <summary>
	/// ImGuiの表示
	/// </summary>
	void DisplayImGui();

	///-------------------------------------------/// 
	/// ゲッター・セッター
	///-------------------------------------------///
public:

	/// <summary>
	/// ワールドトランスフォームのゲッター
	/// </summary>
	/// <returns>ワールドトランスフォーム</returns>
	WorldTransform& GetWorldTransform() { return transform_; }

	/// <summary>
	/// モデルのゲッター
	/// </summary>
	/// <returns>モデル</returns>
	Model* GetModel() const { return model_;}

	/// <summary>
	/// カメラのセッター
	/// </summary>
	/// <param name="camera">カメラ</param>
	void SetCamera(Camera* camera) { camera_ = camera; }

	/// <summary>
	/// モデルのセッター
	/// </summary>
	/// <param name="modelName">ファイル名</param>
	void SetModel(const std::string& modelName);

	///-------------------------------------------/// 
	/// メンバ構造体
	///-------------------------------------------///
private:

	//座標変換行列データ
	struct TransformationMatrix {
		Matrix4x4 WVP;
		Matrix4x4 World;
		Matrix4x4 WorldInverseTranspose;
	};

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//3Dオブジェクト基底
	Object3DCommon* object3DCommon_;

	//カメラ
	Camera* camera_;

	//座標データ
	WorldTransform transform_;

	//バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> WVPResource_ = nullptr;

	//バッファリソース内のデータを指すポインタ
	TransformationMatrix* WVPData_ = nullptr;

	//モデル情報
	Model* model_;

	//軸方向ライン
	std::vector<std::unique_ptr<DebugLine>> axisLines_;

	bool isDebug_;
};