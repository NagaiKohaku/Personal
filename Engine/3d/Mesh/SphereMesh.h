#pragma once

#include "3d/Mesh/MeshBase.h"

///=====================================================/// 
/// 球体メッシュクラス
///=====================================================///
class SphereMesh : public MeshBase {

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

private:

	//緯度の分割数
	const uint32_t kLatitudeCount = 16;

	//経度の分割数
	const uint32_t kLongitudeCount = 16;

	//中心点
	Vector3 center_ = { 0.0f, 0.0f, 0.0f };

	//半径
	float radius_ = 1.0f;
};