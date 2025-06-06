#pragma once

#include "3d/Primitive/PrimitiveBase.h"

#include "numbers"

class Ring : public PrimitiveBase {

public:

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

private:

	const uint32_t kRingDivide = 32;
	const float kOuterRadius = 1.0f;
	const float kInnerRadius = 0.2f;
	const float radianPerDivide = 2.0f * std::numbers::pi_v<float> / static_cast<float>(kRingDivide);
};