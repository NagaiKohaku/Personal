#pragma once

#include "2d/Object/Object2D.h"
#include "3d/Object/Object3D.h"

#include "memory"

/// === 前方宣言 === ///

class MyEngine::Camera;

/// <summary>
/// レティクルを管理するクラスです。
/// </summary>
class Reticle {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// レティクルを初期化します。
	/// </summary>
	/// <param name="cameraPtr">描画やスクリーン座標変換に使用するカメラのポインタです。</param>
	void Initialize(MyEngine::Object2DCommon* object2DCommonPtr, MyEngine::Object3DCommon* object3DCommonPtr, MyEngine::DebugObjectCommon* debugObjectCommonPtr, MyEngine::Camera* cameraPtr);

	/// <summary>
	/// レティクルの更新処理を行います。
	/// </summary>
	void Update();

	/// <summary>
	/// レティクルの描画処理を行います。
	/// </summary>
	void Draw();

	/// <summary>
	/// アニメーションタイマーをリセットします。
	/// </summary>
	void ResetAnimTimer() { animTimer_ = 0.0f; }

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//カメラ
	MyEngine::Camera* camera_;

	//2Dレティクル
	std::unique_ptr<MyEngine::Object2D> object2D_;

	//3Dレティクル
	std::unique_ptr<MyEngine::Object3D> object3D_;

	//2Dレティクルの描画フラグ
	bool isDraw2D_;

	//3Dレティクルの描画フラグ
	bool isDraw3D_;

	//アニメーションタイマー
	float animTimer_;

	//アニメーション最大時間
	float animMaxTime_;

	///-------------------------------------------///
	/// ゲッター・セッター
	///-------------------------------------------///
public:

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns>座標</returns>
	MyEngine::Vector3 GetWorldPos() const { return object3D_->GetWorldTransform().GetWorldTranslate(); }

	/// <summary>
	/// ターゲット座標の設定
	/// </summary>
	/// <param name="targetPos">座標</param>
	void SetTargetPos(MyEngine::Vector3 targetPos) { object3D_->GetWorldTransform().translate_ = targetPos; }
};