#pragma once

#include "3d/Mesh/MeshBase.h"

#include "Math/Vector3.h"

///=====================================================/// 
/// 立方体メッシュクラス
///=====================================================///
class CubeMesh : public MeshBase {

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

private:

	//サイズ
	Vector3 size_;
};