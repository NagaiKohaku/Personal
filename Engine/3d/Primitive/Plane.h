#pragma once

#include "3d/Primitive/PrimitiveBase.h"

class Plane : public PrimitiveBase {

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

	Vector3 anchorPoint_ = { 0.5f,0.5f,0.0f };

};