#include "MeshType.h"

#include "3d/Model/Model.h"

#include "3d/Mesh/PlaneMesh.h"
#include "3d/Mesh/RingMesh.h"
#include "3d/Mesh/CylinderMesh.h"
#include "3d/Mesh/SphereMesh.h"
#include "3d/Mesh/CubeMesh.h"
#include "3d/Mesh/ModelMesh.h"

///=====================================================/// 
/// メッシュの生成
///=====================================================///
std::unique_ptr<MeshBase> CreateMesh(MeshType type) {

	switch (type) {
	case MeshType::PLANE:
		return std::make_unique<PlaneMesh>();
	case MeshType::RING:
		return std::make_unique<RingMesh>();
	case MeshType::CYLINDER:
		return std::make_unique<CylinderMesh>();
	case MeshType::SPHERE:
		return std::make_unique<SphereMesh>();
	case MeshType::CUBE:
		return std::make_unique<CubeMesh>();
	case MeshType::MODEL:
		return std::make_unique<ModelMesh>();
	default:
		return std::make_unique<PlaneMesh>();
	}
}

///=====================================================/// 
/// メッシュの種類を取得
///=====================================================///
MeshType GetMeshType(Model* model) {

	if (dynamic_cast<ModelMesh*>(model->GetMesh())) {
		return MeshType::MODEL;
	}
	if (dynamic_cast<PlaneMesh*>(model->GetMesh())) {
		return MeshType::PLANE;
	}
	if (dynamic_cast<RingMesh*>(model->GetMesh())) {
		return MeshType::RING;
	}
	if (dynamic_cast<CylinderMesh*>(model->GetMesh())) {
		return MeshType::CYLINDER;
	}
	if (dynamic_cast<CubeMesh*>(model->GetMesh())) {
		return MeshType::CUBE;
	}
	if (dynamic_cast<SphereMesh*>(model->GetMesh())) {
		return MeshType::SPHERE;
	}
	return MeshType::END;
}