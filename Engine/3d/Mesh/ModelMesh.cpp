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
		vertexResource_ = dxCommon_->CreateBufferResource(sizeof(VertexData) * vertexCount_);

		//リソースの先頭のアドレスを取得する
		vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();

		//使用するリソースのサイズを設定
		vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * vertexCount_);

		//1頂点当たりのサイズを設定
		vertexBufferView_.StrideInBytes = sizeof(VertexData);

		//書き込むためのアドレスを取得する
		vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

		/// === インデックスリソース === ///

		//インデックスリソースを作成
		indexResource_ = dxCommon_->CreateBufferResource(sizeof(uint32_t) * indexCount_);

		//リソースの先頭アドレスを取得
		indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();

		//使用するリソースのサイズを設定
		indexBufferView_.SizeInBytes = UINT(sizeof(uint32_t) * indexCount_);

		//フォーマットを設定
		indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

		//書き込むためのアドレスを取得する
		indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));

	}
}