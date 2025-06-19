#pragma once

#include "3d/Primitive/PrimitiveBase.h"

class Model;

enum PrimitiveType {
	PLANE,
	RING,
	CYLINDER,
	BALL,
	MODEL,
	END
};

std::unique_ptr<PrimitiveBase> CreatePrimitive(PrimitiveType type);

PrimitiveType GetPrimitiveType(Model* model);