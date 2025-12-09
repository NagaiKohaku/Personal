#include "CubeMesh.h"

///=====================================================/// 
/// CubeMeshの初期化
///=====================================================///
void CubeMesh::Initialize() {

	/// === 頂点データの設定 === ///

	// 右面 (+X) - CCW
	vertexData_.push_back({
		{ 1.0f, 1.0f, -1.0f, 1.0f },
		{ 1.0f, 0.0f },
		{ 1.0f, 0.0f }
		});

	vertexData_.push_back({
		{ 1.0f, 1.0f,  1.0f, 1.0f },
		{ 1.0f, 1.0f },
		{ 1.0f, 0.0f, 0.0f }
		});

	vertexData_.push_back({
		{ 1.0f,-1.0f, -1.0f, 1.0f },
		{ 0.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f }
		});

	vertexData_.push_back({
		{ 1.0f,-1.0f,  1.0f, 1.0f },
		{ 0.0f, 1.0f },
		{ 1.0f, 0.0f, 0.0f }
		});

	// 左面 (−X) - CCW
	vertexData_.push_back({
		{ -1.0f, 1.0f,  1.0f, 1.0f },
		{ 1.0f, 0.0f },
		{ -1.0f, 0.0f, 0.0f }
		});

	vertexData_.push_back({
		{ -1.0f, 1.0f, -1.0f, 1.0f },
		{ 1.0f, 1.0f },
		{ -1.0f, 0.0f, 0.0f }
		});

	vertexData_.push_back({
		{ -1.0f,-1.0f,  1.0f, 1.0f },
		{ 0.0f, 0.0f },
		{ -1.0f, 0.0f, 0.0f }
		});

	vertexData_.push_back({
		{ -1.0f,-1.0f, -1.0f, 1.0f },
		{ 0.0f, 1.0f },
		{ -1.0f, 0.0f, 0.0f }
		});

	// 前面 (+Z) - CCW
	vertexData_.push_back({
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f }
		});

	vertexData_.push_back({
		{ -1.0f, 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f }
		});

	vertexData_.push_back({
		{ 1.0f,-1.0f, 1.0f, 1.0f },
		{ 0.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f }
		});

	vertexData_.push_back({
		{ -1.0f,-1.0f, 1.0f, 1.0f },
		{ 0.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f }
		});

	// 後面 (−Z) - CCW
	vertexData_.push_back({
		{ -1.0f, 1.0f,-1.0f, 1.0f },
		{ 1.0f, 0.0f },
		{ 0.0f, 0.0f, -1.0f }
		});

	vertexData_.push_back({
		{ 1.0f, 1.0f,-1.0f, 1.0f },
		{ 1.0f, 1.0f },
		{ 0.0f, 0.0f, -1.0f }
		});

	vertexData_.push_back({
		{ -1.0f,-1.0f,-1.0f, 1.0f },
		{ 0.0f, 0.0f },
		{ 0.0f, 0.0f, -1.0f }
		});

	vertexData_.push_back({
		{ 1.0f,-1.0f,-1.0f, 1.0f },
		{ 0.0f, 1.0f },
		{ 0.0f, 0.0f, -1.0f }
		});

	// 上面 (+Y) - CCW
	vertexData_.push_back({
		{ 1.0f, 1.0f,-1.0f, 1.0f },
		{ 1.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f }
		});

	vertexData_.push_back({
		{ -1.0f, 1.0f,-1.0f, 1.0f },
		{ 1.0f, 1.0f },
		{ 0.0f, 1.0f, 0.0f }
		});

	vertexData_.push_back({
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		{ 0.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f }
		});

	vertexData_.push_back({
		{ -1.0f, 1.0f, 1.0f, 1.0f },
		{ 0.0f, 1.0f },
		{ 0.0f, 1.0f, 0.0f }
		});

	// 下面 (−Y) - CCW
	vertexData_.push_back({
		{ 1.0f,-1.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f },
		{ 0.0f, -1.0f, 0.0f }
		});

	vertexData_.push_back({
		{ -1.0f,-1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f },
		{ 0.0f, -1.0f, 0.0f }
		});

	vertexData_.push_back({
		{ 1.0f,-1.0f,-1.0f, 1.0f },
		{ 0.0f, 0.0f },
		{ 0.0f, -1.0f, 0.0f }
		});

	vertexData_.push_back({
		{ -1.0f,-1.0f,-1.0f, 1.0f },
		{ 0.0f, 1.0f },
		{ 0.0f, -1.0f, 0.0f }
		});

	/// === 頂点インデックスデータの設定 === ///

	//頂点インデックスデータの設定
	for (uint32_t index = 0; index < 6; ++index) {

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