#include "CubeMesh.h"

namespace MyEngine {

	///=====================================================/// 
	/// CubeMeshの初期化
	///=====================================================///
	void CubeMesh::Initialize(DirectXCommon* dxCommonPtr) {

		/// === シングルトンインスタンスの取得 === ///

		dxCommon_ = dxCommonPtr;

		/// === 頂点リソースの生成 === ///

		//頂点数の設定
		vertexData_.resize(static_cast<size_t>(4 * 6));

		//頂点リソースの生成
		vertexResource_ = dxCommon_->CreateBufferResource(sizeof(VertexData) * vertexData_.size());

		//頂点バッファビューの作成
		vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();

		//使用するリソースのサイズを設定
		vertexBufferView_.SizeInBytes = sizeof(VertexData) * static_cast<UINT>(vertexData_.size());

		//1頂点当たりのサイズを設定
		vertexBufferView_.StrideInBytes = sizeof(VertexData);

		//リソースにデータを書き込めるようにする
		vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&mappedVertexData_));

		/// === 頂点データの設定 === ///

		// 右面 (+X) - CCW
		vertexData_[0].position = { 1.0f, 1.0f, -1.0f, 1.0f };
		vertexData_[0].texcoord = { 1.0f, 0.0f };
		vertexData_[0].normal = { 1.0f, 0.0f, 0.0f };

		vertexData_[1].position = { 1.0f, 1.0f,  1.0f, 1.0f };
		vertexData_[1].texcoord = { 1.0f, 1.0f };
		vertexData_[1].normal = { 1.0f, 0.0f, 0.0f };

		vertexData_[2].position = { 1.0f,-1.0f, -1.0f, 1.0f };
		vertexData_[2].texcoord = { 0.0f, 0.0f };
		vertexData_[2].normal = { 1.0f, 0.0f, 0.0f };

		vertexData_[3].position = { 1.0f,-1.0f,  1.0f, 1.0f };
		vertexData_[3].texcoord = { 0.0f, 1.0f };
		vertexData_[3].normal = { 1.0f, 0.0f, 0.0f };

		// 左面 (−X) - CCW
		vertexData_[4].position = { -1.0f, 1.0f,  1.0f, 1.0f };
		vertexData_[4].texcoord = { 1.0f, 0.0f };
		vertexData_[4].normal = { -1.0f, 0.0f, 0.0f };

		vertexData_[5].position = { -1.0f, 1.0f, -1.0f, 1.0f };
		vertexData_[5].texcoord = { 1.0f, 1.0f };
		vertexData_[5].normal = { -1.0f, 0.0f, 0.0f };

		vertexData_[6].position = { -1.0f,-1.0f,  1.0f, 1.0f };
		vertexData_[6].texcoord = { 0.0f, 0.0f };
		vertexData_[6].normal = { -1.0f, 0.0f, 0.0f };

		vertexData_[7].position = { -1.0f,-1.0f, -1.0f, 1.0f };
		vertexData_[7].texcoord = { 0.0f, 1.0f };
		vertexData_[7].normal = { -1.0f, 0.0f, 0.0f };

		// 前面 (+Z) - CCW
		vertexData_[8].position = { 1.0f, 1.0f, 1.0f, 1.0f };
		vertexData_[8].texcoord = { 1.0f, 0.0f };
		vertexData_[8].normal = { 0.0f, 0.0f, 1.0f };

		vertexData_[9].position = { -1.0f, 1.0f, 1.0f, 1.0f };
		vertexData_[9].texcoord = { 1.0f, 1.0f };
		vertexData_[9].normal = { 0.0f, 0.0f, 1.0f };

		vertexData_[10].position = { 1.0f,-1.0f, 1.0f, 1.0f };
		vertexData_[10].texcoord = { 0.0f, 0.0f };
		vertexData_[10].normal = { 0.0f, 0.0f, 1.0f };

		vertexData_[11].position = { -1.0f,-1.0f, 1.0f, 1.0f };
		vertexData_[11].texcoord = { 0.0f, 1.0f };
		vertexData_[11].normal = { 0.0f, 0.0f, 1.0f };

		// 後面 (−Z) - CCW
		vertexData_[12].position = { -1.0f, 1.0f,-1.0f, 1.0f };
		vertexData_[12].texcoord = { 1.0f, 0.0f };
		vertexData_[12].normal = { 0.0f, 0.0f, -1.0f };

		vertexData_[13].position = { 1.0f, 1.0f,-1.0f, 1.0f };
		vertexData_[13].texcoord = { 1.0f, 1.0f };
		vertexData_[13].normal = { 0.0f, 0.0f, -1.0f };

		vertexData_[14].position = { -1.0f,-1.0f,-1.0f, 1.0f };
		vertexData_[14].texcoord = { 0.0f, 0.0f };
		vertexData_[14].normal = { 0.0f, 0.0f, -1.0f };

		vertexData_[15].position = { 1.0f,-1.0f,-1.0f, 1.0f };
		vertexData_[15].texcoord = { 0.0f, 1.0f };
		vertexData_[15].normal = { 0.0f, 0.0f, -1.0f };

		// 上面 (+Y) - CCW
		vertexData_[16].position = { 1.0f, 1.0f, -1.0f, 1.0f };
		vertexData_[16].texcoord = { 1.0f, 0.0f };
		vertexData_[16].normal = { 0.0f, 1.0f, 0.0f };

		vertexData_[17].position = { -1.0f, 1.0f, -1.0f, 1.0f };
		vertexData_[17].texcoord = { 1.0f, 1.0f };
		vertexData_[17].normal = { 0.0f, 1.0f, 0.0f };

		vertexData_[18].position = { 1.0f, 1.0f, 1.0f, 1.0f };
		vertexData_[18].texcoord = { 0.0f, 0.0f };
		vertexData_[18].normal = { 0.0f, 1.0f, 0.0f };

		vertexData_[19].position = { -1.0f, 1.0f, 1.0f, 1.0f };
		vertexData_[19].texcoord = { 0.0f, 1.0f };
		vertexData_[19].normal = { 0.0f, 1.0f, 0.0f };

		// 下面 (−Y) - CCW
		vertexData_[20].position = { 1.0f,-1.0f, 1.0f, 1.0f };
		vertexData_[20].texcoord = { 1.0f, 0.0f };
		vertexData_[20].normal = { 0.0f, -1.0f, 0.0f };

		vertexData_[21].position = { -1.0f,-1.0f, 1.0f, 1.0f };
		vertexData_[21].texcoord = { 1.0f, 1.0f };
		vertexData_[21].normal = { 0.0f, -1.0f, 0.0f };

		vertexData_[22].position = { 1.0f,-1.0f,-1.0f, 1.0f };
		vertexData_[22].texcoord = { 0.0f, 0.0f };
		vertexData_[22].normal = { 0.0f, -1.0f, 0.0f };

		vertexData_[23].position = { -1.0f,-1.0f,-1.0f, 1.0f };
		vertexData_[23].texcoord = { 0.0f, 1.0f };
		vertexData_[23].normal = { 0.0f, -1.0f, 0.0f };

		std::memcpy(mappedVertexData_, vertexData_.data(), sizeof(VertexData)* vertexData_.size());

		/// === 頂点インデックスリソースの生成 === ///

		//頂点インデックス数の設定
		indexData_.resize(static_cast<size_t>(6 * 6));

		//頂点インデックスリソースの生成
		indexResource_ = dxCommon_->CreateBufferResource(sizeof(uint32_t) * indexData_.size());

		//リソースの場所を取得
		indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();

		//使用するリソースのサイズを設定
		indexBufferView_.SizeInBytes = sizeof(uint32_t) * static_cast<UINT>(indexData_.size());

		//フォーマットを設定
		indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

		//リソースにデータを書き込めるようにする
		indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&mappedIndexData_));

		//頂点インデックスデータの設定
		for (uint32_t index = 0; index < 6; ++index) {
			indexData_[index * 6 + 0] = index * 4 + 0;
			indexData_[index * 6 + 1] = index * 4 + 1;
			indexData_[index * 6 + 2] = index * 4 + 2;
			indexData_[index * 6 + 3] = index * 4 + 1;
			indexData_[index * 6 + 4] = index * 4 + 3;
			indexData_[index * 6 + 5] = index * 4 + 2;
		}

		std::memcpy(mappedIndexData_, indexData_.data(), sizeof(uint32_t) * static_cast<UINT>(indexData_.size()));

	}
}