#pragma once

#include "Collider.h"

#include "Math/Sphere.h"

class SphereCollider : public Collider{

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WorldTransform& parent) override;
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

public:

	Sphere GetSphere() const { return sphere_; }

private:

	/// <summary>
	/// デバッグラインの生成
	/// </summary>
	void CreateDebugLines() override;

private:

	Sphere sphere_;
};