#include "MeshBase.h"

void MeshBase::Initialize() {

	/// === シングルトンインスタンスの取得 === ///

	directXCommon_ = DirectXCommon::GetInstance();

	/// === 頂点リソースの生成 === ///

	//頂点リソースの生成
	vertexResource_ = directXCommon_->CreateBufferResource(sizeof(VertexData) * vertexData_.size());

	//頂点バッファビューの作成
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();

	//使用するリソースのサイズを設定
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * static_cast<UINT>(vertexData_.size());

	//1頂点当たりのサイズを設定
	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	//リソースにデータを書き込めるようにする
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&mappedVertexData_));

	//頂点データをリソースにコピー
	memcpy(mappedVertexData_, vertexData_.data(), sizeof(VertexData) * vertexData_.size());

	/// === 頂点インデックスリソースの生成 === ///

	//頂点インデックスリソースの生成
	indexResource_ = directXCommon_->CreateBufferResource(sizeof(uint32_t) * indexData_.size());

	//リソースの場所を取得
	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();

	//使用するリソースのサイズを設定
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * static_cast<UINT>(indexData_.size());

	//フォーマットを設定
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	//リソースにデータを書き込めるようにする
	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&mappedIndexData_));

	//頂点インデックスデータをリソースにコピー
	memcpy(mappedIndexData_, indexData_.data(), sizeof(uint32_t) * indexData_.size());
}

///=====================================================/// 
/// GPUにメッシュのデータを送信
///=====================================================///
void MeshBase::SendDataForGPU() {

	//頂点データをGPUに転送
	directXCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);

	//インデックスデータをGPUに転送
	directXCommon_->GetCommandList()->IASetIndexBuffer(&indexBufferView_);
}

///=====================================================/// 
/// 外部から頂点データとインデックスデータをコピーしてメッシュを更新
///=====================================================///
void MeshBase::CopyMeshData(std::vector<uint32_t> indices, std::vector<VertexData> vertices) {

	vertexData_ = vertices;

	indexData_ = indices;
}

///=====================================================/// 
/// GPU上のメッシュデータを更新
///=====================================================///
void MeshBase::UpdateMeshDataGPU() {

	//頂点データをリソースにコピー
	memcpy(mappedVertexData_, vertexData_.data(), sizeof(VertexData) * vertexData_.size());

	//頂点インデックスデータをリソースにコピー
	memcpy(mappedIndexData_, indexData_.data(), sizeof(uint32_t) * indexData_.size());
}

///=====================================================///
/// 頂点データを追加
///=====================================================///
void MeshBase::AddVertexData(const VertexData& vertex) {

	indexData_.push_back(static_cast<uint32_t>(vertexData_.size()));

	vertexData_.push_back(vertex);
}