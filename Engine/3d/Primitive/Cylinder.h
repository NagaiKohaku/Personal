#pragma once

#include "3d/Primitive/PrimitiveBase.h"

#include "numbers"

class Cylinder : public PrimitiveBase {

public:

	void Initialize() override;

	void Draw() override;

private:

	const uint32_t kCylinderDivide = 32;
	const float kTopRadius = 1.0f;
	const float kBottomRadius = 1.0f;
	const float kHeight = 3.0f;
	const float radianPerDivide = 2.0f * std::numbers::pi_v<float> / static_cast<float>(kCylinderDivide);
};