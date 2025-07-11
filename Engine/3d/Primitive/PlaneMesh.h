#pragma once

#include "3d/Primitive/MeshBase.h"

class PlaneMesh : public MeshBase {

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

	Vector3 anchorPoint_ = { 0.5f,0.5f,0.0f };

};