#pragma once

#include "3d/Object/Object3D.h"

#include "memory"

class Player {

public:

	void Initialize();

	void Update();

	void Draw();

private:

	void Move();

private:

	std::unique_ptr<Object3D> player_ = nullptr;
};