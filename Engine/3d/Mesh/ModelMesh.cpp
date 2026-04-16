#include "ModelMesh.h"

#include "Base/DirectXCommon.h"

namespace MyEngine {

	///=====================================================/// 
	/// ModelMeshの初期化
	///=====================================================///
	void ModelMesh::Initialize(DirectXCommon* dxCommonPtr) {

		dxCommon_ = dxCommonPtr;

		/// === 頂点リソースの生成 === ///

		//頂点リソースの生成
		vertexResource_ = dxCommon_->CreateBufferResource(sizeof(VertexData) * vertexData_.size());

		//リソースの先頭のアドレスを取得する
		vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();

		//使用するリソースのサイズを設定
		vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * vertexData_.size());

		//1頂点当たりのサイズを設定
		vertexBufferView_.StrideInBytes = sizeof(VertexData);

		//リソースにデータを書き込めるようにする
		vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&mappedVertexData_));

		std::memcpy(mappedVertexData_, vertexData_.data(), sizeof(VertexData) * vertexData_.size());

		/// === インデックスリソース === ///

		//インデックスリソースを作成
		indexResource_ = dxCommon_->CreateBufferResource(sizeof(uint32_t) * indexData_.size());

		//リソースの先頭アドレスを取得
		indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();

		//使用するリソースのサイズを設定
		indexBufferView_.SizeInBytes = UINT(sizeof(uint32_t) * indexData_.size());

		//フォーマットを設定
		indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

		//リソースにデータを書き込めるようにする
		indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&mappedIndexData_));

		std::memcpy(mappedIndexData_, indexData_.data(), sizeof(uint32_t) * indexData_.size());

	}
}