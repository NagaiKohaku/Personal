#pragma once

#include "3d/Mesh/MeshBase.h"

#include "numbers"

/// <summary>
/// 円柱メッシュ（Cylinder Mesh）を管理するクラスです。
/// </summary>
class CylinderMesh : public MeshBase {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// CylinderMeshの初期化を行います。
	/// </summary>
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