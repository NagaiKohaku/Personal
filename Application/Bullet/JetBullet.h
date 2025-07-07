#pragma once

#include "Bullet/BulletBase.h"

#include "3d/Collider/SphereCollider.h"

#include "memory"

class JetBullet : public BulletBase {

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

private:

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// 接触時判定
	/// </summary>
	void IsCollision();

private:

	//コライダー
	std::unique_ptr<SphereCollider> collider_;

};