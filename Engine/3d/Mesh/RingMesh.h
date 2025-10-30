#pragma once

#include "3d/Mesh/MeshBase.h"

#include "numbers"

/// <summary>
/// 円環メッシュ（Ring Mesh）を管理するクラスです。
/// </summary>
/// <remarks>
/// - MeshBase を継承しており、基本的なメッシュ機能を利用可能です。
/// - 円環で表現されて、頂点データとインデックスデータを保有します。
/// </remarks>
class RingMesh : public MeshBase {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// RingMeshの初期化を行います。
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

	//分割数
	const uint32_t kRingDivide = 32;

	//外円の半径
	const float kOuterRadius = 1.0f;

	//内円の半径
	const float kInnerRadius = 0.2f;

	////1分割あたりの円周の半径
	const float radianPerDivide = 2.0f * std::numbers::pi_v<float> / static_cast<float>(kRingDivide);
};