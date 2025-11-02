#pragma once

#include "Base/LayerType.h"
#include "3d/Object/DebugLine.h"

#include "LevelEditor/ObjectData.h"

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

/// === 前方宣言 === ///

class Object3DCommon;

class Model;

class Camera;

/// <summary>
/// 3D空間上のオブジェクトを管理・描画するクラスです。
/// </summary>
/// <remarks>
/// - 空間内での位置、回転、拡大縮小などの変換情報を管理します。  
/// - 3Dモデルを内部に保持し、描画処理を通じて画面に表示します。  
/// - デバッグ用として、オブジェクトの方向を示す軸ラインを生成し表示できます。  
/// </remarks>
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
	/// <remarks>
	/// - 3Dオブジェクト共通の管理クラスからインスタンスを取得します。
	/// - 座標変換用の行列リソースを生成し、初期値として単位行列を設定します。
	/// - オブジェクトの位置・回転・スケールを管理するワールドトランスフォームを初期化します。
	/// - デバッグ表示用の軸ラインを3方向（X、Y、Z）作成します。
	/// - 初期化用のモデルを設定します。
	/// </remarks>
	void Initialize();

	/// <summary>
	/// オブジェクトデータを基に3Dオブジェクトの初期化を行います。
	/// </summary>
	/// <param name="objectData">オブジェクトデータ</param>
	/// <remarks>
	/// - 3Dオブジェクト共通の管理クラスからインスタンスを取得します。
	/// - 座標変換用の行列リソースを生成し、初期値として単位行列を設定します。
	/// - オブジェクトの位置・回転・スケールを管理するワールドトランスフォームを初期化します。
	/// - デバッグ表示用の軸ラインを3方向（X、Y、Z）作成します。
	/// - オブジェクトデータを基にモデル・トランスフォームを設定します。
	/// </remarks>
	void Initialize(ObjectData objectData);

	/// <summary>
	/// 3Dオブジェクトの状態を更新します。
	/// </summary>
	/// <remarks>
	/// - オブジェクトの位置・回転・スケールに基づいてトランスフォームを更新します。
	/// - カメラが設定されている場合は、ワールドビュープロジェクション行列を計算します。
	/// - デバッグ表示用に作成された3方向の軸ラインもそれぞれ更新します。
	/// </remarks>
	void Update();

	/// <summary>
	/// 3Dオブジェクトの描画を行います。
	/// </summary>
	/// <param name="layer">描画するレイヤー</param>
	/// <remarks>
	/// - 描画コマンドを作成し、描画前処理として共通設定を行います。
	/// - 座標変換行列リソースをGPUに送信します。
	/// - モデルが存在する場合はモデルの描画処理を実行します。
	/// - 描画コマンドをレンダラーに登録して、指定された描画レイヤーに反映させます。
	/// - デバッグモードが有効な場合は、オブジェクトの軸方向を示すラインも描画します。
	/// </remarks>
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