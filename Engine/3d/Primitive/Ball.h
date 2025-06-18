#pragma once

#include "3d/Primitive/PrimitiveBase.h"

class Ball : public PrimitiveBase {

public:

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

	void CopyMeshData(std::vector<uint32_t> indices, std::vector<VertexData> vertices) override;

private:

	// 緯度の分割数
	const uint32_t kLatitudeCount = 16;

	// 経度の分割数
	const uint32_t kLongitudeCount = 16;

	Vector3 center_ = { 0.0f, 0.0f, 0.0f };

	float radius_ = 1.0f; // 半径1の球
};