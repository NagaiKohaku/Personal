#include "SphereMesh.h"

#include "numbers"

///=====================================================/// 
/// SphereMeshの初期化
///=====================================================///
void SphereMesh::Initialize() {

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
			vertexData_.push_back({
				{ std::cosf(lon0) * std::sinf(lat0), std::cosf(lat0), std::sinf(lon0) * std::sinf(lat0), 1.0f},
				{ static_cast<float>(lon) / kLongitudeCount, static_cast<float>(lat) / kLatitudeCount},
				{ std::cosf(lon0) * std::sinf(lat0), std::cosf(lat0), std::sinf(lon0) * std::sinf(lat0)}
				});

			//右下
			vertexData_.push_back({
				{ std::cosf(lon1) * std::sinf(lat0), std::cosf(lat0), std::sinf(lon1) * std::sinf(lat0), 1.0f},
				{ static_cast<float>(lon + 1) / kLongitudeCount, static_cast<float>(lat) / kLatitudeCount},
				{ std::cosf(lon1) * std::sinf(lat0), std::cosf(lat0), std::sinf(lon1) * std::sinf(lat0)}
				});

			//左上
			vertexData_.push_back({
				{ std::cosf(lon0) * std::sinf(lat1), std::cosf(lat1), std::sinf(lon0) * std::sinf(lat1), 1.0f},
				{ static_cast<float>(lon) / kLongitudeCount, static_cast<float>(lat + 1) / kLatitudeCount},
				{ std::cosf(lon0) * std::sinf(lat1), std::cosf(lat1), std::sinf(lon0) * std::sinf(lat1)}
				});

			//右上
			vertexData_.push_back({
				{ std::cosf(lon1) * std::sinf(lat1), std::cosf(lat1), std::sinf(lon1) * std::sinf(lat1), 1.0f},
				{ static_cast<float>(lon + 1) / kLongitudeCount, static_cast<float>(lat + 1) / kLatitudeCount},
				{ std::cosf(lon1) * std::sinf(lat1), std::cosf(lat1), std::sinf(lon1) * std::sinf(lat1)}
				});

			//1面分の計算が終わったので頂点数分進める
			vertexIndex += 4;
		}
	}

	/// === 頂点インデックスデータの生成 === ///

	for (uint32_t index = 0; index < kLatitudeCount * kLongitudeCount; ++index) {

		indexData_.push_back(index * 4 + 0);
		indexData_.push_back(index * 4 + 1);
		indexData_.push_back(index * 4 + 2);
		indexData_.push_back(index * 4 + 1);
		indexData_.push_back(index * 4 + 3);
		indexData_.push_back(index * 4 + 2);
	}

	//リソースの初期化
	MeshBase::Initialize();
}