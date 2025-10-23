#pragma once

#include "3d/Mesh/MeshBase.h"

/// <summary>
/// 球面体メッシュ（Sphere Mesh）を管理するクラスです。
/// </summary>
/// <remarks>
/// - MeshBase を継承しており、基本的なメッシュ機能を利用可能です。
/// - 球面体で表現されて、頂点データとインデックスデータを保有します。
/// </remarks>
class SphereMesh : public MeshBase {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// SphereMeshの初期化を行います。
	/// </summary>
	/// <remarks>
	/// - 頂点リソースとインデックスリソースを生成します。
	/// - 頂点データには位置・法線・UVを設定します。
	/// - インデックスデータは各面を2つの三角形で描画するために構成します。
	/// </remarks>
	void Initialize() override;

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
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