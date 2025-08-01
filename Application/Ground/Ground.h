#pragma once

#include "memory"

class Object3D;

class Ground {

public:

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

private:

	std::unique_ptr<Object3D> object_;

	float uvTimer_;
};