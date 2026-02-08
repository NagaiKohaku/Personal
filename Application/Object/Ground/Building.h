#pragma once

#include "memory"

#include "3d/Object/Object3D.h"

#include <Math/Vector/Vector3.h>
#include <Math/Transform/WorldTransform.h>

/// <summary>
/// 建物オブジェクトを管理するクラスです。
/// </summary>
class Building {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// 建物オブジェクトを初期化します。
	/// </summary>
	/// <param name="pos">建物の配置位置を指定します。</param>
	/// <param name="scale">建物のスケール（大きさ）を指定します。</param>
	void Initialize(MyEngine::Vector3 pos, MyEngine::Vector3 scale);

	/// <summary>
	/// 建物オブジェクトの更新処理を行います。
	/// </summary>
	void Update();

	/// <summary>
	/// 建物オブジェクトの座標更新を行います。
	/// </summary>
	void TransformUpdate();

	/// <summary>
	/// 建物オブジェクトを描画します。
	/// </summary>
	void Draw();

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//建物オブジェクト
	std::unique_ptr<MyEngine::Object3D> object_;

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
	MyEngine::WorldTransform GetWorldTransform(){
		return object_->GetWorldTransform();
	}

	/// <summary>
	/// 座標を設定
	/// </summary>
	/// <param name="pos">座標</param>
	void SetPosition(MyEngine::Vector3 pos) {
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