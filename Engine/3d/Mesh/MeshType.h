#pragma once

#include "3d/Mesh/MeshBase.h"

class Model;

enum MeshType {
	PLANE,
	RING,
	CYLINDER,
	SPHERE,
	MODEL,
	END
};

std::unique_ptr<MeshBase> CreateMesh(MeshType type);

MeshType GetMeshType(Model* model);