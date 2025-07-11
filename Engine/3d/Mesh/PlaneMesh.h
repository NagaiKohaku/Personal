#pragma once

#include "3d/Mesh/MeshBase.h"

class PlaneMesh : public MeshBase {

public:

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize() override;

private:

	Vector3 anchorPoint_ = { 0.5f,0.5f,0.0f };

};