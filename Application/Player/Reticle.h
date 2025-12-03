#pragma once

#include "2d/Object/Object2D.h"
#include "3d/Object/Object3D.h"

#include "memory"

/// === 前方宣言 === ///

class Camera;

/// <summary>
/// レティクルを管理するクラスです。
/// </summary>
/// <remarks>
/// - 2Dおよび3Dの表示形式を持ち、UI上での描画やゲーム内オブジェクトとしての位置を管理します。  
/// - 起動アニメーションや更新処理を行い、3D座標とスクリーン座標の同期を自動で行います。  
/// - 描画フラグに応じて2Dまたは3Dオブジェクトを描画し、状態に応じた表示切替をサポートします。  
/// - アニメーションタイマーを保持し、必要に応じてリセットできます。  
/// </remarks>
class Reticle {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// レティクルを初期化します。
	/// </summary>
	/// <param name="cameraPtr">描画やスクリーン座標変換に使用するカメラのポインタです。</param>
	/// <remarks>
	/// - 2Dスプライトとして使用するレティクルを生成・初期化し、サイズやアンカーポイントを設定します。  
	/// - 3Dオブジェクトとして使用するレティクルも生成・初期化し、モデルを設定します。  
	/// - 描画フラグを設定し、2D描画を有効、3D描画を無効にします。  
	/// - アニメーション用のタイマーを初期化し、最大アニメーション時間を設定します。  
	/// </remarks>
	void Initialize(Camera* cameraPtr);

	/// <summary>
	/// レティクルの更新処理を行います。
	/// </summary>
	/// <remarks>
	/// - 起動時のアニメーションタイマーを進め、0から1への補間で2Dスプライトの色を変化させます。  
	/// - 3Dオブジェクトの位置や状態を更新します。  
	/// - 3Dオブジェクトのワールド座標をスクリーン座標に変換し、2Dオブジェクトの位置を同期させます。  
	/// - 2Dオブジェクトの状態も更新し、描画準備を行います。  
	/// </remarks>
	void Update();

	/// <summary>
	/// レティクルの描画処理を行います。
	/// </summary>
	/// <remarks>
	/// - 2D描画フラグが有効な場合、UIレイヤーとして2Dオブジェクトを描画します。  
	/// - 3D描画フラグが有効な場合、オブジェクトレイヤーとして3Dオブジェクトを描画します。  
	/// - 状態に応じて2D・3D描画を切り替えながら表示を行います。  
	/// </remarks>
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
	Camera* camera_;

	//2Dレティクル
	std::unique_ptr<Object2D> object2D_;

	//3Dレティクル
	std::unique_ptr<Object3D> object3D_;

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
	Vector3 GetWorldPos() const { return object3D_->GetWorldTransform().GetWorldTranslate(); }

	/// <summary>
	/// ターゲット座標の設定
	/// </summary>
	/// <param name="targetPos">座標</param>
	void SetTargetPos(Vector3 targetPos) { object3D_->GetWorldTransform().translate_ = targetPos; }
};