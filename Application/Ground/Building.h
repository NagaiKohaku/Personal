#pragma once

#include "memory"

#include "3d/Object/Object3D.h"

#include "Math/Vector3.h"
#include "Math/WorldTransform.h"

/// <summary>
/// 建物オブジェクトを管理するクラスです。
/// </summary>
/// <remarks>
/// - 内部に3Dオブジェクトを保持し、初期化・更新・描画処理を行います。
/// - 建物の位置やスケール、移動速度を設定可能です。
/// </remarks>
class Building {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// 建物オブジェクトを初期化します。
	/// </summary>
	/// <param name="pos">建物の配置位置を指定します。</param>
	/// <param name="rotate">建物の配置角度を指定します。</param>
	/// <remarks>
	/// - 3Dオブジェクトを生成し、初期化します。
	/// - キューブモデルを設定し、指定された位置と角度を適用します。
	/// - 建物の移動速度を初期値として設定します。
	/// </remarks>
	void Initialize(Vector3 pos, Vector3 rotate);

	/// <summary>
	/// 建物オブジェクトの更新処理を行います。
	/// </summary>
	/// <remarks>
	/// - 内部の3Dオブジェクトの更新処理を呼び出します。
	/// - 建物をZ方向に速度に応じて移動させます。
	/// </remarks>
	void Update();

	/// <summary>
	/// 建物オブジェクトの座標更新を行います。
	/// </summary>
	/// <remarks>
	/// - 内部の3Dオブジェクトの更新処理を呼び出し、ワールド変換を反映させます。
	/// </remarks>
	void TransformUpdate();

	/// <summary>
	/// 建物オブジェクトを描画します。
	/// </summary>
	/// <remarks>
	/// - 内部の3Dオブジェクトをオブジェクトレイヤーに描画します。
	/// </remarks>
	void Draw();

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//建物オブジェクト
	std::unique_ptr<Object3D> object_;

	//移動速度
	float speed_;

	///-------------------------------------------/// 
	/// ゲッター・セッター
	///-------------------------------------------///
public:

	/// <summary>
	/// ワールドトランスフォームを取得
	/// </summary>
	/// <returns>ワールドトランスフォーム</returns>
	WorldTransform GetWorldTransform(){
		return object_->GetWorldTransform();
	}

	/// <summary>
	/// 座標を設定
	/// </summary>
	/// <param name="pos">座標</param>
	void SetPosition(Vector3 pos) {
		object_->GetWorldTransform().translate_ = pos;
	}

	/// <summary>
	/// Z座標を設定
	/// </summary>
	/// <param name="z">Z座標</param>
	void SetPosZ(float z) {
		object_->GetWorldTransform().translate_.z = z;
	}
};