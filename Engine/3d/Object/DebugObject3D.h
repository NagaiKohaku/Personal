#pragma once

#include "3d/Model/Model.h"


#include "Math/Matrix4x4.h"
#include "Math/WorldTransform.h"

#include "wrl.h"

class DebugObjectCommon;

class Camera;

class DebugObject3D {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

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
	Model* GetModel() const { return model_.get(); }

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
	/// メンバ変数
	///-------------------------------------------///
private:

	//デバッグオブジェクト基底
	DebugObjectCommon* debugCommon_;

	//カメラ
	Camera* camera_;

	//座標データ
	WorldTransform transform_;

	//バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> WVPResource_ = nullptr;

	//バッファリソース内のデータを指すポインタ
	TransformationMatrix* WVPData_ = nullptr;

	//モデル情報
	std::unique_ptr<Model> model_;
};