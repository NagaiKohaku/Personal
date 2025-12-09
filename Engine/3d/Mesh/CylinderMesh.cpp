#include "CylinderMesh.h"

///=====================================================/// 
/// CylinderMeshの初期化
///=====================================================///
void CylinderMesh::Initialize() {

	/// === 頂点データの設定 === ///

	for (uint32_t index = 0; index < kCylinderDivide; ++index) {

		float sin = std::sinf(index * radianPerDivide);
		float cos = std::cosf(index * radianPerDivide);
		float sinNext = std::sinf((index + 1) * radianPerDivide);
		float cosNext = std::cosf((index + 1) * radianPerDivide);
		float u = static_cast<float>(index) / static_cast<float>(kCylinderDivide);
		float uNext = static_cast<float>(index + 1) / static_cast<float>(kCylinderDivide);

		//面の4頂点を設定
		vertexData_.push_back({
			{ -sin * kTopRadius,kHeight,cos * kTopRadius,1.0f },
			{ u,0.0f },
			{ -sin,0.0f,cos }
			});

		vertexData_.push_back({
			{ -sinNext * kTopRadius,kHeight,cosNext * kTopRadius,1.0f },
			{ uNext,0.0f },
			{ -sinNext,0.0f,cosNext }
			});

		vertexData_.push_back({
			{ -sin * kBottomRadius,0.0f,cos * kBottomRadius,1.0f },
			{ u,1.0f },
			{ -sin,0.0f,cos }
			});

		vertexData_.push_back({
			{ -sinNext * kBottomRadius,0.0f,cosNext * kBottomRadius,1.0f },
			{ uNext,1.0f },
			{ -sinNext,0.0f,cosNext }
			});
	}

	/// === 頂点インデックスデータの設定 === ///

	for (uint32_t index = 0; index < kCylinderDivide; ++index) {

		indexData_.push_back(index * 4 + 0);
		indexData_.push_back(index * 4 + 1);
		indexData_.push_back(index * 4 + 2);
		indexData_.push_back(index * 4 + 1);
		indexData_.push_back(index * 4 + 3);
		indexData_.push_back(index * 4 + 2);
	}

	//リソースの初期化
	MeshBase::Initialize();
}