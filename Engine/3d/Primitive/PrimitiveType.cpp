#include "PrimitiveType.h"

#include "3d/Model/Model.h"

#include "3d/Primitive/Plane.h"
#include "3d/Primitive/Ring.h"
#include "3d/Primitive/Cylinder.h"
#include "3d/Primitive/Ball.h"
#include "3d/Primitive/ModelMesh.h"

std::unique_ptr<PrimitiveBase> CreatePrimitive(PrimitiveType type) {

	switch (type) {
	case PrimitiveType::PLANE:
		return std::make_unique<Plane>();
	case PrimitiveType::RING:
		return std::make_unique<Ring>();
	case PrimitiveType::CYLINDER:
		return std::make_unique<Cylinder>();
	case PrimitiveType::BALL:
		return std::make_unique<Ball>();
	case PrimitiveType::MODEL:
		return std::make_unique<ModelMesh>();
	default:
		return std::make_unique<Plane>();
	}
}

PrimitiveType GetPrimitiveType(Model* model) {

	if (dynamic_cast<ModelMesh*>(model->GetPrimitive())) {
		return PrimitiveType::MODEL;
	}
	if (dynamic_cast<Plane*>(model->GetPrimitive())) {
		return PrimitiveType::PLANE;
	}
	if (dynamic_cast<Ring*>(model->GetPrimitive())) {
		return PrimitiveType::RING;
	}
	if (dynamic_cast<Cylinder*>(model->GetPrimitive())) {
		return PrimitiveType::CYLINDER;
	}
	if (dynamic_cast<Ball*>(model->GetPrimitive())) {
		return PrimitiveType::BALL;
	}
	return PrimitiveType::END;
}