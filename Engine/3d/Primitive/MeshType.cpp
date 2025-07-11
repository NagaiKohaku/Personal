#include "MeshType.h"

#include "3d/Model/Model.h"

#include "3d/Primitive/PlaneMesh.h"
#include "3d/Primitive/RingMesh.h"
#include "3d/Primitive/CylinderMesh.h"
#include "3d/Primitive/SphereMesh.h"
#include "3d/Primitive/ModelMesh.h"

std::unique_ptr<MeshBase> CreatePrimitive(MeshType type) {

	switch (type) {
	case MeshType::PLANE:
		return std::make_unique<PlaneMesh>();
	case MeshType::RING:
		return std::make_unique<RingMesh>();
	case MeshType::CYLINDER:
		return std::make_unique<CylinderMesh>();
	case MeshType::SPHERE:
		return std::make_unique<SphereMesh>();
	case MeshType::MODEL:
		return std::make_unique<ModelMesh>();
	default:
		return std::make_unique<PlaneMesh>();
	}
}

MeshType GetPrimitiveType(Model* model) {

	if (dynamic_cast<ModelMesh*>(model->GetPrimitive())) {
		return MeshType::MODEL;
	}
	if (dynamic_cast<PlaneMesh*>(model->GetPrimitive())) {
		return MeshType::PLANE;
	}
	if (dynamic_cast<RingMesh*>(model->GetPrimitive())) {
		return MeshType::RING;
	}
	if (dynamic_cast<CylinderMesh*>(model->GetPrimitive())) {
		return MeshType::CYLINDER;
	}
	if (dynamic_cast<SphereMesh*>(model->GetPrimitive())) {
		return MeshType::SPHERE;
	}
	return MeshType::END;
}