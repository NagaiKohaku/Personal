#include "RingMesh.h"

///=====================================================/// 
/// RingMeshの初期化
///=====================================================///
void RingMesh::Initialize() {

	/// === 頂点データの設定 === ///

	for (uint32_t index = 0; index < kRingDivide; ++index) {

		float sin = std::sinf(index * radianPerDivide);
		float cos = std::cosf(index * radianPerDivide);
		float sinNext = std::sinf((index + 1) * radianPerDivide);
		float cosNext = std::cosf((index + 1) * radianPerDivide);

		float u = static_cast<float>(index) / static_cast<float>(kRingDivide);
		float uNext = static_cast<float>(index + 1) / static_cast<float>(kRingDivide);

		vertexData_.push_back({
			{ -sin * kOuterRadius,cos * kOuterRadius,0.0f,1.0f },
			{ u,0.0f },
			{ 0.0f,0.0f,-1.0f }
			});

		vertexData_.push_back({
			{ -sinNext * kOuterRadius,cosNext * kOuterRadius,0.0f,1.0f },
			{ uNext,0.0f },
			{ 0.0f,0.0f,-1.0f }
			});

		vertexData_.push_back({
			{ -sin * kInnerRadius,cos * kInnerRadius,0.0f,1.0f },
			{ u,1.0f },
			{ 0.0f,0.0f,-1.0f }
			});

		vertexData_.push_back({
			{ -sinNext * kInnerRadius,cosNext * kInnerRadius,0.0f,1.0f },
			{ uNext,1.0f },
			{ 0.0f,0.0f,-1.0f }
			});
	}

	/// === 頂点インデックスデータの設定 === ///

	for (uint32_t index = 0; index < kRingDivide; ++index) {

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