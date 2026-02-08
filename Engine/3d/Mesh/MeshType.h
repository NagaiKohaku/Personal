#pragma once

#include "3d/Mesh/MeshBase.h"

namespace MyEngine {

	class Model;

	//メッシュの種類を列挙型で定義
	enum class MeshType {
		PLANE,
		RING,
		CYLINDER,
		SPHERE,
		CUBE,
		MODEL,
		END
	};

	/// <summary>
	/// 指定されたメッシュタイプに応じてMeshBase派生クラスのインスタンスを生成します。
	/// </summary>
	/// <param name="type">生成するメッシュの種類</param>
	/// <returns>生成されたMeshBase派生クラスのunique_ptr</returns>
	std::unique_ptr<MeshBase> CreateMesh(MeshType type);

	/// <summary>
	/// Model に紐づくメッシュの種類を取得します。
	/// </summary>
	/// <param name="model">メッシュを持つ Model インスタンス</param>
	/// <returns>該当する MeshType、該当なしの場合は MeshType::END</returns>
	MeshType GetMeshType(Model* model);
}