#include "PlaneMesh.h"

#include "Base/DirectXCommon.h"

namespace MyEngine {

	///=====================================================/// 
	/// PlaneMeshの初期化
	///=====================================================///
	void PlaneMesh::Initialize(DirectXCommon* dxCommonPtr) {

		dxCommon_ = dxCommonPtr;

		/// === 頂点リソースの生成 === ///

		vertexData_.resize(static_cast<size_t>(4));

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

		//頂点データの初期化
		//左下
		vertexData_[0].position = { 0.0f - anchorPoint_.x,1.0f - anchorPoint_.y,0.0f - anchorPoint_.z,1.0f };
		vertexData_[0].texcoord = { 0.0f,1.0f };
		vertexData_[0].normal = { 0.0f,0.0f,-1.0f };
		//左上
		vertexData_[1].position = { 0.0f - anchorPoint_.x,0.0f - anchorPoint_.y,0.0f - anchorPoint_.z,1.0f };
		vertexData_[1].texcoord = { 0.0f,0.0f };
		vertexData_[1].normal = { 0.0f,0.0f,-1.0f };
		//右下
		vertexData_[2].position = { 1.0f - anchorPoint_.x,1.0f - anchorPoint_.y,0.0f - anchorPoint_.z,1.0f };
		vertexData_[2].texcoord = { 1.0f,1.0f };
		vertexData_[2].normal = { 0.0f,0.0f,-1.0f };
		//右上
		vertexData_[3].position = { 1.0f - anchorPoint_.x,0.0f - anchorPoint_.y,0.0f - anchorPoint_.z,1.0f };
		vertexData_[3].texcoord = { 1.0f,0.0f };
		vertexData_[3].normal = { 0.0f,0.0f,-1.0f };

		std::memcpy(mappedVertexData_, vertexData_.data(), sizeof(VertexData) * vertexData_.size());

		/// === 頂点インデックスリソースの生成 === ///

		indexData_.resize(static_cast<size_t>(6));

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

		//頂点インデックスデータの初期化
		indexData_[0] = 0;
		indexData_[1] = 1;
		indexData_[2] = 2;
		indexData_[3] = 1;
		indexData_[4] = 3;
		indexData_[5] = 2;

		std::memcpy(mappedIndexData_, indexData_.data(), sizeof(uint32_t) * indexData_.size());

	}
}