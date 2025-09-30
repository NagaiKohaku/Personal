#pragma once

#include "memory";

#include "3d/Object/Object3D.h"

#include "Math/Vector3.h"

class Shadow {

public:
	// 初期化
	void Initialize();

	// 更新
	void Update(Vector3 pos);

	// 描画
	void Draw();

private:

	std::unique_ptr<Object3D> shadow_;

	float maxLength_;

	float maxScale_;

	bool isDraw_;

};