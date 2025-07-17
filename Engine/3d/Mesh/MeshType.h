#pragma once

#include "3d/Mesh/MeshBase.h"

/// === 前方宣言 === ///

class Model;

//メッシュの種類を列挙型で定義
enum MeshType {
	PLANE,
	RING,
	CYLINDER,
	SPHERE,
	CUBE,
	MODEL,
	END
};

/// <summary>
/// メッシュの生成
/// </summary>
/// <param name="type"></param>
/// <returns></returns>
std::unique_ptr<MeshBase> CreateMesh(MeshType type);

/// <summary>
/// メッシュの種類を取得
/// </summary>
/// <param name="model"></param>
/// <returns></returns>
MeshType GetMeshType(Model* model);