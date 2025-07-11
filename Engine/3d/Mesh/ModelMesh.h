#pragma once

#include "3d/Mesh/MeshBase.h"

class DirectXCommon;

class ModelMesh : public MeshBase {

public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize() override;
};