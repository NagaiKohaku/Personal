#pragma once

#include "memory"

#include "Math/Vector3.h"
#include "Math/WorldTransform.h"

class Object3D;

class Building {

public:

	// 初期化
	void Initialize(Vector3 pos, Vector3 scale);

	// 更新
	void Update();

	// 座標のみ更新
	void TransformUpdate();

	// 描画
	void Draw();

public:

	WorldTransform GetWorldTransform();

	void SetPosition(Vector3 pos);

	void SetPosZ(float z);

private:

	std::unique_ptr<Object3D> object_;

	float speed_;

};