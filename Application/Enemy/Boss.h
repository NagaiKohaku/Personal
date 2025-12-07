#pragma once

#include "3d/Object/Object3D.h"

#include "memory"

class Boss {

public:

	void Initialize();

	void Update();

	void Draw();

private:

	//オブジェクト
	std::unique_ptr<Object3D> object_ = nullptr;

	WorldTransform barrier01Transform_;

	WorldTransform barrier02Transform_;

	WorldTransform barrier03Transform_;

	WorldTransform barrier04Transform_;

};