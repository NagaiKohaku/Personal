#pragma once

#include "memory"

class Object3D;

class Ground {

public:

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 座標のみ更新
	void TransformUpdate();

	// 描画
	void Draw();

private:

	std::unique_ptr<Object3D> object_;

	float uvTimer_;
};