#pragma once

#include "3d/Mesh/MeshBase.h"

#include "numbers"

class RingMesh : public MeshBase {

public:

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize() override;

private:

	const uint32_t kRingDivide = 32;
	const float kOuterRadius = 1.0f;
	const float kInnerRadius = 0.2f;
	const float radianPerDivide = 2.0f * std::numbers::pi_v<float> / static_cast<float>(kRingDivide);
};