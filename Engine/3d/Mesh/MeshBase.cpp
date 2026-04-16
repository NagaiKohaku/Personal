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

	void MeshBase::AddVertexData(const VertexData& vertex) {

		indexData_.push_back(static_cast<uint32_t>(vertexData_.size()));

		vertexData_.push_back(vertex);
	}
}