#pragma once

#include "3d/Primitive/MeshBase.h"

class DirectXCommon;

class ModelMesh : public MeshBase {

public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

	void CopyMeshData(std::vector<uint32_t> indices, std::vector<VertexData> vertices) override;

private:

	DirectXCommon* directXCommon_;

	//バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource_ = nullptr;

	//バッファリソース内のデータを指すポインタ
	VertexData* vertexData_ = nullptr;
	uint32_t* indexData_ = nullptr;

	//バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	D3D12_INDEX_BUFFER_VIEW indexBufferView_;
};