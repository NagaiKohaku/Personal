#include "MeshBase.h"

namespace MyEngine {

	///=====================================================/// 
	/// GPUにメッシュのデータを送信
	///=====================================================///
	void MeshBase::SendDataForGPU() {

		//頂点データをGPUに転送
		dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);

		//インデックスデータをGPUに転送
		dxCommon_->GetCommandList()->IASetIndexBuffer(&indexBufferView_);
	}

	///=====================================================/// 
	/// 外部から頂点データとインデックスデータをコピーしてメッシュを更新
	///=====================================================///
	void MeshBase::CopyMeshData(std::vector<uint32_t> indices, std::vector<VertexData> vertices) {
		//vertexData_に頂点データをコピー
		std::memcpy(vertexData_, vertices.data(), sizeof(VertexData) * vertices.size());

		//indexData_にインデックスデータをコピー
		std::memcpy(indexData_, indices.data(), sizeof(uint32_t) * indices.size());
	}
}