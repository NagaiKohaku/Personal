#pragma once

#include "2d/Object/Object2D.h"

class Flash {

public:

	static Flash* GetInstance();

	void Initialize();

	void Update();

	void Draw();

	void Start(float maxTime, Vector4 color);

private:

	std::unique_ptr<Object2D> flashObject_;

	float alpha_;

	bool isFlash_;

	float timer_;

	float maxTime_;

	Vector4 color_;
};