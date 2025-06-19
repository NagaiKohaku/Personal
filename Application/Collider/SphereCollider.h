#pragma once

#include "Collider.h"

#include "3d/Primitive/Ball.h"

#include "Math/Sphere.h"

class SphereCollider : public Collider{

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WorldTransform* parent) override;
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

	void Remove();

public:

	Sphere GetSphere() const { return sphere_; }

	void SetCenter(Vector3 center) { sphere_.center = center; }

	void SetRadius(float radius) { sphere_.radius = radius; }

private:

	Sphere sphere_;

};