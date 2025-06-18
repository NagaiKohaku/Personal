#include "ModelMesh.h"

#include "Base/DirectXCommon.h"

void ModelMesh::Initialize() {

	directXCommon_ = DirectXCommon::GetInstance();

	/// === 頂点リソースの生成 === ///

	//頂点リソースの生成
	vertexResource_ = directXCommon_->CreateBufferResource(sizeof(VertexData) * vertexCount_);

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
	indexResource_ = directXCommon_->CreateBufferResource(sizeof(uint32_t) * indexCount_);

	//リソースの先頭アドレスを取得
	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();

	//使用するリソースのサイズを設定
	indexBufferView_.SizeInBytes = UINT(sizeof(uint32_t) * indexCount_);

	//フォーマットを設定
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	//書き込むためのアドレスを取得する
	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));

}

void ModelMesh::Draw() {

	//頂点データの設定
	directXCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);

	//頂点番号の設定
	directXCommon_->GetCommandList()->IASetIndexBuffer(&indexBufferView_);

}

void ModelMesh::CopyMeshData(std::vector<uint32_t> indices, std::vector<VertexData> vertices) {

	std::memcpy(vertexData_, vertices.data(), sizeof(VertexData) * vertices.size());
	std::memcpy(indexData_, indices.data(), sizeof(uint32_t) * indices.size());
}