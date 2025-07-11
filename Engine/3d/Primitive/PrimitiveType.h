#pragma once

#include "3d/Primitive/PrimitiveBase.h"

class Model;

enum PrimitiveType {
	PLANE,
	RING,
	CYLINDER,
	SPHERE,
	MODEL,
	END
};

std::unique_ptr<PrimitiveBase> CreatePrimitive(PrimitiveType type);

PrimitiveType GetPrimitiveType(Model* model);