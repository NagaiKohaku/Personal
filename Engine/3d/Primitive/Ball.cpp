#include "Ball.h"

#include "numbers"

void Ball::Initialize() {

	directXCommon_ = DirectXCommon::GetInstance();

	/// === 頂点リソースの生成 === ///

	vertexCount_ = (kLatitudeCount + 1) * (kLongitudeCount + 1);

	//頂点リソースの生成
	vertexResource_ = directXCommon_->CreateBufferResource(sizeof(VertexData) * 4 * vertexCount_);

	//頂点バッファビューの作成
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();

	//使用するリソースのサイズを設定
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * 4 * vertexCount_;

	//1頂点当たりのサイズを設定
	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	//リソースにデータを書き込めるようにする
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	uint32_t vertexIndex = 0;
	for (uint32_t lat = 0; lat < kLatitudeCount; ++lat) {

		for (uint32_t lon = 0; lon < kLongitudeCount; ++lon) {

			float lat0 = std::numbers::pi_v<float> * static_cast<float>(lat) / static_cast<float>(kLatitudeCount);
			float lat1 = std::numbers::pi_v<float> *static_cast<float>(lat + 1) / static_cast<float>(kLatitudeCount);
			float lon0 = 2.0f * std::numbers::pi_v<float> *static_cast<float>(lon) / static_cast<float>(kLongitudeCount);
			float lon1 = 2.0f * std::numbers::pi_v<float> *static_cast<float>(lon + 1) / static_cast<float>(kLongitudeCount);

			// 球の表面上の点を計算
			vertexData_[vertexIndex + 0].position = {
				std::cosf(lon0) * std::sinf(lat0),
				std::cosf(lat0),
				std::sinf(lon0) * std::sinf(lat0),
				1.0f
			};
			vertexData_[vertexIndex + 0].texcoord = {
				static_cast<float>(lon) / kLongitudeCount,
				static_cast<float>(lat) / kLatitudeCount
			};
			vertexData_[vertexIndex + 0].normal = {
				std::cosf(lon0) * std::sinf(lat0),
				std::cosf(lat0),
				std::sinf(lon0) * std::sinf(lat0)
			};

			vertexData_[vertexIndex + 1].position = {
				std::cosf(lon1) * std::sinf(lat0),
				std::cosf(lat0),
				std::sinf(lon1) * std::sinf(lat0),
				1.0f
			};
			vertexData_[vertexIndex + 1].texcoord = {
				static_cast<float>(lon + 1) / kLongitudeCount,
				static_cast<float>(lat) / kLatitudeCount
			};
			vertexData_[vertexIndex + 1].normal = {
				std::cosf(lon1)* std::sinf(lat0),
				std::cosf(lat0),
				std::sinf(lon1)* std::sinf(lat0)
			};

			vertexData_[vertexIndex + 2].position = {
				std::cosf(lon0) * std::sinf(lat1),
				std::cosf(lat1),
				std::sinf(lon0) * std::sinf(lat1),
				1.0f
			};
			vertexData_[vertexIndex + 2].texcoord = {
				static_cast<float>(lon) / kLongitudeCount,
				static_cast<float>(lat + 1) / kLatitudeCount
			};
			vertexData_[vertexIndex + 2].normal = {
				std::cosf(lon0)* std::sinf(lat1),
				std::cosf(lat1),
				std::sinf(lon0)* std::sinf(lat1)
			};

			vertexData_[vertexIndex + 3].position = {
				std::cosf(lon1) * std::sinf(lat1),
				std::cosf(lat1),
				std::sinf(lon1) * std::sinf(lat1),
				1.0f
			};
			vertexData_[vertexIndex + 3].texcoord = {
				static_cast<float>(lon + 1) / kLongitudeCount,
				static_cast<float>(lat + 1) / kLatitudeCount
			};
			vertexData_[vertexIndex + 3].normal = {
				std::cosf(lon1) * std::sinf(lat1),
				std::cosf(lat1),
				std::sinf(lon1) * std::sinf(lat1)
			};
			
			vertexIndex += 4;
		}
	}

	/// === 頂点インデックスリソースの生成 === ///

	indexCount_ = 6 * kLatitudeCount * kLongitudeCount;

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

	for (uint32_t index = 0; index < kLatitudeCount * kLongitudeCount; ++index) {
		indexData_[index * 6 + 0] = index * 4 + 0;
		indexData_[index * 6 + 1] = index * 4 + 1;
		indexData_[index * 6 + 2] = index * 4 + 2;
		indexData_[index * 6 + 3] = index * 4 + 1;
		indexData_[index * 6 + 4] = index * 4 + 3;
		indexData_[index * 6 + 5] = index * 4 + 2;
	}
}

void Ball::Draw() {

	//VBVを設定
	directXCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);

	directXCommon_->GetCommandList()->IASetIndexBuffer(&indexBufferView_);
}

void Ball::CopyMeshData(std::vector<uint32_t> indices, std::vector<VertexData> vertices) {

	std::memcpy(vertexData_, vertices.data(), sizeof(VertexData) * vertices.size());
	std::memcpy(indexData_, indices.data(), sizeof(uint32_t) * indices.size());
}