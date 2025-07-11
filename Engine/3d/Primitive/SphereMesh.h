#pragma once

#include "3d/Primitive/PrimitiveBase.h"

///=====================================================/// 
/// 球体メッシュクラス
///=====================================================///
class SphereMesh : public PrimitiveBase {

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 頂点データ、インデックスデータのコピー
	/// </summary>
	/// <param name="indices">コピー先のインデックスデータ</param>
	/// <param name="vertices">コピー先の頂点データ</param>
	void CopyMeshData(std::vector<uint32_t> indices, std::vector<VertexData> vertices) override;

private:

	//緯度の分割数
	const uint32_t kLatitudeCount = 16;

	//経度の分割数
	const uint32_t kLongitudeCount = 16;

	//中心点
	Vector3 center_ = { 0.0f, 0.0f, 0.0f };

	//半径
	float radius_ = 1.0f;
};