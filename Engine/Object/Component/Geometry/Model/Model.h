#pragma once

#include <Object/Component/ComponentBase.h>
#include <Object/3D/Model/ModelCommon.h>
#include <Object/3D/Mesh/MeshBase.h>
#include <Object/3D/Mesh/MeshType.h>
#include <Object/3D/Material/ModelMaterial.h>
#include <Object/2D/Texture/Texture.h>
#include <Math/Vector/Vector4.h>
#include <Math/Matrix/Matrix4x4.h>

#include <vector>
#include <memory>
#include <unordered_map>
#include <string>

namespace MyEngine {

	class Model {

	public:

		struct ModelPart {
			std::string name;
			std::string materialName;
			std::string textureFilePath;
			std::unique_ptr<MeshBase> mesh;
			std::unique_ptr<ModelMaterial> material;
			std::unique_ptr<Texture> texture;
		};

	public:

		void Initialize(ModelCommon* modelCommon, MeshType type, const std::string& filePath);

		void Draw();

	private:

		const std::string kResourceDirectoryPath_ = "Resource/Model/";

		ModelCommon* modelCommon_;

		std::string modelDirectoryPath_;

		std::vector<ModelPart> modelParts_;

	private:

		void LoadObjFile(const std::string& fileName);

		Vector4 LoadVertexPosition(std::istringstream& stream);

		Vector2 LoadTexCoord(std::istringstream& stream);

		Vector3 LoadNormal(std::istringstream& stream);

		std::vector<MeshBase::VertexData> LoadTriangleFace(
			std::istringstream& stream,
			const std::vector<Vector4>& positions,
			const std::vector<Vector2>& texcoords,
			const std::vector<Vector3>& normals
		);

		std::unordered_map<std::string, std::string> LoadMaterialTemplateFile(const std::string& fileName);

	public:

		std::vector<ModelPart>& GetModelParts() { return modelParts_; }

		MeshBase* GetMesh(size_t index = 0) { return modelParts_[index].mesh.get(); }

		ModelMaterial* GetMaterial(size_t index = 0) { return modelParts_[index].material.get(); }

		Texture* GetTexture(size_t index = 0) { return modelParts_[index].texture.get(); }
	};
}