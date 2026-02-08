#include "SphereMesh.h"

#include "numbers"

///=====================================================/// 
/// SphereMeshの初期化
///=====================================================///
void SphereMesh::Initialize() {

	/// === シングルトンインスタンスの取得 === ///

	directXCommon_ = MyEngine::DirectXCommon::GetInstance();

	/// === 頂点リソースの生成 === ///

	//分割数から頂点数を計算
	vertexCount_ = (kLatitudeCount + 1) * (kLongitudeCount + 1);

	//リソースの生成
	vertexResource_ = directXCommon_->CreateBufferResource(sizeof(VertexData) * 4 * vertexCount_);

	//バッファビューの作成
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();

	//リソースのサイズを設定
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * 4 * vertexCount_;

	//1頂点当たりのサイズを設定
	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	//リソースにデータを書き込めるようにする
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	/// === 頂点データの生成 === ///

	//頂点番号
	uint32_t vertexIndex = 0;

	//緯度と経度の分割数に基づいて頂点データを生成
	for (uint32_t lat = 0; lat < kLatitudeCount; ++lat) {

		for (uint32_t lon = 0; lon < kLongitudeCount; ++lon) {

			//緯度と経度から頂点の位置を計算
			float lat0 = std::numbers::pi_v<float> *static_cast<float>(lat) / static_cast<float>(kLatitudeCount);
			float lon0 = 2.0f * std::numbers::pi_v<float> *static_cast<float>(lon) / static_cast<float>(kLongitudeCount);

			//次の緯度と経度の位置を計算
			float lat1 = std::numbers::pi_v<float> *static_cast<float>(lat + 1) / static_cast<float>(kLatitudeCount);
			float lon1 = 2.0f * std::numbers::pi_v<float> *static_cast<float>(lon + 1) / static_cast<float>(kLongitudeCount);

			//球の分割された1面の4つの頂点を計算
			//左下
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

			//右下
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
				std::cosf(lon1) * std::sinf(lat0),
				std::cosf(lat0),
				std::sinf(lon1) * std::sinf(lat0)
			};

			//左上
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
				std::cosf(lon0) * std::sinf(lat1),
				std::cosf(lat1),
				std::sinf(lon0) * std::sinf(lat1)
			};

			//右上
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

			//1面分の計算が終わったので頂点数分進める
			vertexIndex += 4;
		}
	}

	/// === 頂点インデックスリソースの生成 === ///

	//分割数からインデックス数を計算
	indexCount_ = 6 * kLatitudeCount * kLongitudeCount;

	//リソースの生成
	indexResource_ = directXCommon_->CreateBufferResource(sizeof(uint32_t) * indexCount_);

	//リソースの場所を取得
	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();

	//使用するリソースのサイズを設定
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * indexCount_;

	//フォーマットを設定
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	//リソースにデータを書き込めるようにする
	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));

	/// === インデックスデータの生成 === ///

	for (uint32_t index = 0; index < kLatitudeCount * kLongitudeCount; ++index) {

		//球の分割された1面の6つの頂点番号を設定
		//左下
		indexData_[index * 6 + 0] = index * 4 + 0;

		//右下
		indexData_[index * 6 + 1] = index * 4 + 1;

		//左上
		indexData_[index * 6 + 2] = index * 4 + 2;

		//右下
		indexData_[index * 6 + 3] = index * 4 + 1;

		//右上
		indexData_[index * 6 + 4] = index * 4 + 3;

		//左上
		indexData_[index * 6 + 5] = index * 4 + 2;
	}
}