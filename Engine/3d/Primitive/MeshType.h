#pragma once

#include "3d/Primitive/MeshBase.h"

class Model;

enum MeshType {
	PLANE,
	RING,
	CYLINDER,
	SPHERE,
	MODEL,
	END
};

std::unique_ptr<MeshBase> CreatePrimitive(MeshType type);

MeshType GetPrimitiveType(Model* model);