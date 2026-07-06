#pragma once

#include <3d/Object/Object3D.h>
#include <3d/Collider/AABBCollider.h>

#include <Math/Vector/Vector3.h>

#include <vector>
#include <memory>

class DamageGround {

public:

	~DamageGround();

	void Initialize(MyEngine::Vector3 pos);

	void Update();

	void TransformUpdate();

	void Draw();

	bool GetIsDead() const { return isDead_; }

private:

	std::unique_ptr<MyEngine::Object3D> object_;

	//コライダー
	std::unique_ptr<MyEngine::AABBCollider> collider_;

	float speed_;

	float deadLine_;

	bool isDead_;

};