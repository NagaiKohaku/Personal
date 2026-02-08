#pragma once

#include <Object/Bullet/Core/BulletBase.h>

#include "3d/Collider/SphereCollider.h"

#include "memory"

/// <summary>
/// 飛行機状態の弾を管理するクラスです。
/// </summary>
class JetBullet : public BulletBase {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// 弾が破棄される際に関連するコライダーを削除します。
	/// </summary>
	~JetBullet();

	/// <summary>
	/// 弾を初期化し、パラメータを設定します。
	/// </summary>
	/// <param name="pos">生成時の座標を指定します。</param>
	/// <param name="direction">移動方向を指定します。</param>
	void Initialize(MyEngine::Vector3 pos, MyEngine::Vector3 direction) override;

	/// <summary>
	/// 弾の状態を更新します
	/// </summary>
	void Update() override;

	/// <summary>
	/// 弾の座標更新処理を行います
	/// </summary>
	void TransformUpdate() override;

	/// <summary>
	/// 弾の描画処理を行います
	/// </summary>
	void Draw() override;

	///-------------------------------------------/// 
	/// クラス内処理関数
	///-------------------------------------------///
private:

	/// <summary>
	/// 弾の移動処理を行います
	/// </summary>
	void Move();

	/// <summary>
	/// 他オブジェクトと接触している場合の処理を行います
	/// </summary>
	void IsCollision();

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//コライダー
	std::unique_ptr<MyEngine::SphereCollider> collider_;

};