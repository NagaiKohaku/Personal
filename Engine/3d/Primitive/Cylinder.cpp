#include "Cylinder.h"

void Cylinder::Initialize() {

	directXCommon_ = DirectXCommon::GetInstance();

	/// === 頂点リソースの生成 === ///

	//頂点リソースの生成
	vertexResource_ = directXCommon_->CreateBufferResource(sizeof(VertexData) * 4 * kCylinderDivide);

	//頂点バッファビューの作成
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();

	//使用するリソースのサイズを設定
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * 4 * kCylinderDivide;

	//1頂点当たりのサイズを設定
	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	//リソースにデータを書き込めるようにする
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	for (uint32_t index = 0; index < kCylinderDivide; ++index) {

		float sin = std::sinf(index * radianPerDivide);
		float cos = std::cosf(index * radianPerDivide);
		float sinNext = std::sinf((index + 1) * radianPerDivide);
		float cosNext = std::cosf((index + 1) * radianPerDivide);
		float u = static_cast<float>(index) / static_cast<float>(kCylinderDivide);
		float uNext = static_cast<float>(index + 1) / static_cast<float>(kCylinderDivide);

		vertexData_[index * 4 + 0].position = { -sin * kTopRadius,kHeight,cos * kTopRadius,1.0f };
		vertexData_[index * 4 + 0].texcoord = { u,0.0f };
		vertexData_[index * 4 + 0].normal = { -sin,0.0f,cos };

		vertexData_[index * 4 + 1].position = { -sinNext * kTopRadius,kHeight,cosNext * kTopRadius,1.0f };
		vertexData_[index * 4 + 1].texcoord = { uNext,0.0f };
		vertexData_[index * 4 + 1].normal = { -sinNext,0.0f,cosNext };

		vertexData_[index * 4 + 2].position = { -sin * kBottomRadius,0.0f,cos * kBottomRadius,1.0f };
		vertexData_[index * 4 + 2].texcoord = { u,1.0f };
		vertexData_[index * 4 + 2].normal = { -sin,0.0f,cos };

		vertexData_[index * 4 + 3].position = { -sinNext * kBottomRadius,0.0f,cosNext * kBottomRadius,1.0f };
		vertexData_[index * 4 + 3].texcoord = { uNext,1.0f };
		vertexData_[index * 4 + 3].normal = { -sinNext,0.0f,cosNext };
	}

	/// === 頂点インデックスリソースの生成 === ///

	indexCount_ = 6 * kCylinderDivide;

	//頂点インデックスリソースの生成
	IndexResource_ = directXCommon_->CreateBufferResource(sizeof(uint32_t) * indexCount_);

	//リソースの場所を取得
	indexBufferView_.BufferLocation = IndexResource_->GetGPUVirtualAddress();

	//使用するリソースのサイズを設定
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * indexCount_;

	//フォーマットを設定
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	//リソースにデータを書き込めるようにする
	IndexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));

	for (uint32_t index = 0; index < kCylinderDivide; ++index) {
		indexData_[index * 6 + 0] = index * 4 + 0;
		indexData_[index * 6 + 1] = index * 4 + 1;
		indexData_[index * 6 + 2] = index * 4 + 2;
		indexData_[index * 6 + 3] = index * 4 + 1;
		indexData_[index * 6 + 4] = index * 4 + 3;
		indexData_[index * 6 + 5] = index * 4 + 2;
	}
}

void Cylinder::Draw() {

	//VBVを設定
	directXCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);

	directXCommon_->GetCommandList()->IASetIndexBuffer(&indexBufferView_);
}