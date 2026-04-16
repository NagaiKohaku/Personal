#pragma once

#include <3d/Model/ModelCommon.h>
#include <3d/Mesh/MeshBase.h>
#include <3d/Mesh/MeshType.h>
#include <Math/Vector/Vector4.h>
#include <Math/Matrix/Matrix4x4.h>

#include <vector>
#include <memory>
#include <unordered_map>
#include <string>

namespace MyEngine {

	class Model {

	public:

		//マテリアル
		struct Material {
			Vector4 color;
			int32_t enableLighting;
			float padding[3];
			Matrix4x4 uvTransform;
			float shininess;
			float environmentCoefficient;
		};

		struct ModelPart {
			std::string name;
			std::string materialName;
			std::unique_ptr<MeshBase> mesh;
			Material* material;
			Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;
			std::string textureFilePath;
			uint32_t textureIndex;
		};

	public:

		~Model();

		void Initialize(ModelCommon* modelCommon, MeshType type, const std::string& filePath);

		void Draw();

	private:

		const std::string kResourceDirectoryPath_ = "Resource/Model/";

		const std::string kDefaultTexterPath_ = "Resource/Texture/white_128x128.png";

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

		std::string GetTextureFilePath(size_t index = 0) const { return modelParts_[index].textureFilePath; }

		

		void SetTextureFilePath(const std::string filePath);
	};
}