#pragma once

#include "3d/Object/DebugLine.h"

#include "memory"
#include "list"

class LineGround {

public:

	void Initialize();

	void Update();

	void Draw();

private:

	void Move();

private:

	//ライン
	std::list<std::unique_ptr<DebugLine>> verticalLines_;

	std::list<std::unique_ptr<DebugLine>> horizontalLines_;

	float lineDivide_ = 30.0f;

	float lineDistance_ = 5.0f;

	float speed_;
};