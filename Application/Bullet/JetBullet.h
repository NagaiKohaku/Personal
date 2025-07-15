#pragma once

#include "Bullet/BulletBase.h"

#include "3d/Collider/SphereCollider.h"

#include "memory"

///=====================================================/// 
/// ジェット弾クラス
///=====================================================///
class JetBullet : public BulletBase {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~JetBullet();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="pos">初期座標</param>
	/// <param name="direction">発射方向</param>
	void Initialize(Vector3 pos, Vector3 direction) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	///-------------------------------------------/// 
	/// クラス内処理関数
	///-------------------------------------------///
private:

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// 接触時判定
	/// </summary>
	void IsCollision();

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//コライダー
	std::unique_ptr<SphereCollider> collider_;

};