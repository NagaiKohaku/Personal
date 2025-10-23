#pragma once

#include "3d/Mesh/MeshBase.h"

#include "numbers"

/// <summary>
/// 円柱メッシュ（Cylinder Mesh）を管理するクラスです。
/// </summary>
/// <remarks>
/// - MeshBase を継承しており、基本的なメッシュ機能を利用可能です。
/// - 円柱で表現されて、頂点データとインデックスデータを保有します。
/// </remarks>
class CylinderMesh : public MeshBase {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// CylinderMeshの初期化を行います。
	/// </summary>
	/// <remarks>
	/// - 円柱の側面の頂点リソースとインデックスリソースを生成します。
	/// - 頂点データには位置・法線・UVを設定します。
	/// - インデックスデータは各面を2つの三角形で描画するために構成します。
	/// </remarks>
	void Initialize() override;

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//面の分割数
	const uint32_t kCylinderDivide = 32;

	//上面の半径
	const float kTopRadius = 1.0f;

	//底面の半径
	const float kBottomRadius = 1.0f;

	//高さ
	const float kHeight = 3.0f;

	//1分割あたりの円周の半径
	const float radianPerDivide = 2.0f * std::numbers::pi_v<float> / static_cast<float>(kCylinderDivide);
};