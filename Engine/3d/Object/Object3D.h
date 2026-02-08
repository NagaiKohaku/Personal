#pragma once

#include "Base/LayerType.h"
#include "3d/Object/DebugLine.h"

#include "LevelEditor/ObjectData.h"

#include <Math/Vector/Vector3.h>
#include <Math/Vector/Vector4.h>
#include <Math/Matrix/Matrix4x4.h>
#include <Math/Transform/WorldTransform.h>

#include "DirectXTex.h"
#include "d3d12.h"

#include "3d/Model/Model.h"

#include "wrl.h"
#include "string"
#include "vector"
#include "memory"

/// === 前方宣言 === ///

class Object3DCommon;

class Camera;

/// <summary>
/// 3D空間上のオブジェクトを管理・描画するクラスです。
/// </summary>
class Object3D {

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
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// 3Dオブジェクトの初期化を行います。
	/// </summary>
	void Initialize();

	/// <summary>
	/// オブジェクトデータを基に3Dオブジェクトの初期化を行います。
	/// </summary>
	/// <param name="objectData">オブジェクトデータ</param>
	void Initialize(ObjectData objectData);

	/// <summary>
	/// 3Dオブジェクトの状態を更新します。
	/// </summary>
	void Update();

	void TransformUpdate();

	/// <summary>
	/// 3Dオブジェクトの描画を行います。
	/// </summary>
	/// <param name="layer">描画するレイヤー</param>
	void Draw(LayerType layer);

	/// <summary>
	/// ImGuiの表示
	/// </summary>
	void DisplayImGui();

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

	//座標変換行列リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> WVPResource_ = nullptr;

	//座標変換行列データ
	TransformationMatrix* WVPData_ = nullptr;

	//モデル
	std::unique_ptr<Model> model_;

	//軸方向ライン
	std::vector<std::unique_ptr<DebugLine>> axisLines_;

	//デバッグフラグ
	bool isDebug_;

	///-------------------------------------------/// 
	/// ゲッター・セッター
	///-------------------------------------------///
public:

	/// <summary>
	/// ワールドトランスフォームを取得
	/// </summary>
	/// <returns>ワールドトランスフォーム</returns>
	WorldTransform& GetWorldTransform() { return transform_; }

	/// <summary>
	/// モデルを取得
	/// </summary>
	/// <returns>モデル</returns>
	Model* GetModel() const { return model_.get(); }

	/// <summary>
	/// カメラの設定
	/// </summary>
	/// <param name="camera">カメラ</param>
	void SetCamera(Camera* camera) { camera_ = camera; }

	/// <summary>
	/// モデルの設定
	/// </summary>
	/// <param name="modelName">ファイル名</param>
	void SetModel(const std::string& modelName);

};