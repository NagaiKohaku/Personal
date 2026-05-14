#include "CylinderMesh.h"

namespace MyEngine {

	///=====================================================/// 
	/// CylinderMeshの初期化
	///=====================================================///
	void CylinderMesh::Initialize(DirectXCommon* dxCommonPtr) {

		/// === シングルトンインスタンスの取得 === ///

		dxCommon_ = dxCommonPtr;

		/// === 頂点リソースの生成 === ///

		vertexData_.resize(static_cast<size_t>(4 * kCylinderDivide));

		//リソースの生成
		vertexResource_ = dxCommon_->CreateBufferResource(sizeof(VertexData) * vertexData_.size());

		//バッファビューの作成
		vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();

		//使用するリソースのサイズを設定
		vertexBufferView_.SizeInBytes = sizeof(VertexData) * static_cast<UINT>(vertexData_.size());

		//1頂点当たりのサイズを設定
		vertexBufferView_.StrideInBytes = sizeof(VertexData);

		//リソースにデータを書き込めるようにする
		vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&mappedVertexData_));

		/// === 頂点データの設定 === ///

		for (uint32_t index = 0; index < kCylinderDivide; ++index) {

			float sin = std::sinf(index * radianPerDivide);
			float cos = std::cosf(index * radianPerDivide);
			float sinNext = std::sinf((index + 1) * radianPerDivide);
			float cosNext = std::cosf((index + 1) * radianPerDivide);
			float u = static_cast<float>(index) / static_cast<float>(kCylinderDivide);
			float uNext = static_cast<float>(index + 1) / static_cast<float>(kCylinderDivide);

			//面の4頂点を設定

			//左下
			vertexData_[index * 4 + 0].position = { -sin * kTopRadius,kHeight,cos * kTopRadius,1.0f };
			vertexData_[index * 4 + 0].texcoord = { u,0.0f };
			vertexData_[index * 4 + 0].normal = { -sin,0.0f,cos };

			//右下
			vertexData_[index * 4 + 1].position = { -sinNext * kTopRadius,kHeight,cosNext * kTopRadius,1.0f };
			vertexData_[index * 4 + 1].texcoord = { uNext,0.0f };
			vertexData_[index * 4 + 1].normal = { -sinNext,0.0f,cosNext };

			//左上
			vertexData_[index * 4 + 2].position = { -sin * kBottomRadius,0.0f,cos * kBottomRadius,1.0f };
			vertexData_[index * 4 + 2].texcoord = { u,1.0f };
			vertexData_[index * 4 + 2].normal = { -sin,0.0f,cos };

			//右上
			vertexData_[index * 4 + 3].position = { -sinNext * kBottomRadius,0.0f,cosNext * kBottomRadius,1.0f };
			vertexData_[index * 4 + 3].texcoord = { uNext,1.0f };
			vertexData_[index * 4 + 3].normal = { -sinNext,0.0f,cosNext };
		}

		std::memcpy(mappedVertexData_, vertexData_.data(), sizeof(VertexData) * vertexData_.size());

		/// === 頂点インデックスリソースの生成 === ///

		indexData_.resize(static_cast<size_t>(6 * kCylinderDivide));

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

		for (uint32_t index = 0; index < kCylinderDivide; ++index) {
			indexData_[index * 6 + 0] = index * 4 + 0;
			indexData_[index * 6 + 1] = index * 4 + 1;
			indexData_[index * 6 + 2] = index * 4 + 2;
			indexData_[index * 6 + 3] = index * 4 + 1;
			indexData_[index * 6 + 4] = index * 4 + 3;
			indexData_[index * 6 + 5] = index * 4 + 2;
		}

		std::memcpy(mappedIndexData_, indexData_.data(), sizeof(uint32_t) * indexData_.size());

	}
}