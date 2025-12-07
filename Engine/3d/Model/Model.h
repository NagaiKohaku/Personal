#pragma once

#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Matrix4x4.h"
#include "Math/MakeMatrixMath.h"
#include "3d/Mesh/MeshBase.h"
#include "3d/Mesh/MeshType.h"
#include "3d/Model/ModelCommon.h"

#include "DirectXTex.h"
#include "d3d12.h"

#include <string>
#include <vector>
#include <wrl.h>

/// <summary>
/// マルチメッシュ・マルチマテリアル対応の Model クラス（置換用）
/// - 既存のエンジンAPI（ModelCommon, TextureManager, MeshBase）に合わせた実装
/// - OBJ の usemtl / mtllib (newmtl / map_Kd) に対応
/// </summary>
class Model {

private:
	// マテリアル（GPUにアップロードする構造）
	struct Material {
		Vector4 color;
		int32_t enableLighting;
		float padding[3];
		Matrix4x4 uvTransform;
		float shininess;
		float environmentCoefficient;
		// テクスチャは別に管理（TextureManager経由）
	};

	// サブメッシュ情報（インデックスの範囲 + マテリアル)
	struct Submesh {
		Matrix4x4 localTransform = MakeIdentity4x4(); // ローカル変換行列
		Matrix4x4 worldTransform = MakeIdentity4x4(); // ワールド変換（親があれば合成）
		// materialIndex, startIndex, indexCount follow

		uint32_t materialIndex = 0;
		uint32_t startIndex = 0;
		uint32_t indexCount = 0;
	};

	// モデルデータ
	struct ModelData {
		std::vector<MeshBase::VertexData> vertices; // 頂点データ
		std::vector<uint32_t> indices; // インデックスデータ

		std::vector<Submesh> submeshes;           // サブメッシュ配列
		std::vector<std::string> materialNames;   // マテリアル名 (usemtl の名前)
		std::vector<std::string> texturePaths;    // マテリアルごとのテクスチャパス (map_Kd)

		std::vector<MeshBase::VertexData> baseVertices;
	};

public:
	void Initialize(const std::string& directoryPath, const std::string& filename);
	void Initialize(MeshType type, const std::string& textureFilePath);
	void Initialize(MeshType type, Model* model);

	void Draw();
	void SendMeshDataForGPU();
	void SendMaterialDataForGPU();
	void SendTextureDataForGPU();
	void Copy(Model* model);

	// ===== Submesh animation API =====
	/// <summary>
	/// サブメッシュのローカル変換を設定します（行列は左乗算で扱います）。
	/// </summary>
	void SetSubmeshLocalTransform(size_t submeshIndex, const Matrix4x4& localTransform);

	/// <summary>
	/// サブメッシュのローカル変換を取得します。
	/// </summary>
	Matrix4x4 GetSubmeshLocalTransform(size_t submeshIndex) const;

	/// <summary>
	/// CPUでサブメッシュ単位の変換を適用し、頂点バッファを更新します。
	/// 毎フレーム呼ぶことでサブメッシュのアニメーションが可能です（GPUスキニング未使用）。
	/// </summary>
	void UpdateSubmeshTransformsCPU();

private:
	void LoadObjFile(const std::string& directoryPath, const std::string& filename);
	void LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);

private:
	ModelCommon* modelCommon_ = nullptr;
	std::unique_ptr<MeshBase> mesh_;

	// Per-material CBV resources
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> materialResources_;
	std::vector<Material*> materialDataMapped_;

	ModelData modelData_;

	// temporary storage used during MTL parsing
	std::unordered_map<std::string, std::string> mtlToTextureMap_;

	// original single-texture compatibility (kept for convenience)
	std::string textureFilePath_;
	uint32_t textureIndex_ = 0;

	public:
	MeshBase* GetMesh() { return mesh_.get(); }
	ModelData GetModelData() { return modelData_; }
	std::string GetTextureFilePath() const { return textureFilePath_; }

	const Vector4& GetColor() { return materialDataMapped_.empty() ? *((Vector4*)nullptr) : materialDataMapped_[0]->color; }
	const Matrix4x4& GetUVTransform() { static Matrix4x4 id; return id; }
	const float GetShininess() { return materialDataMapped_.empty() ? 0.0f : materialDataMapped_[0]->shininess; }
	const float GetEnvironmentCoefficient() { return materialDataMapped_.empty() ? 0.0f : materialDataMapped_[0]->environmentCoefficient; }

	void SetColor(const Vector4& color) { if(!materialDataMapped_.empty()) materialDataMapped_[0]->color = color; }
	void SetUVTransform(const Matrix4x4& uvTransform) { if(!materialDataMapped_.empty()) materialDataMapped_[0]->uvTransform = uvTransform; }
	void SetShininess(const float& shininess) { if(!materialDataMapped_.empty()) materialDataMapped_[0]->shininess = shininess; }
	void SetEnvironmentCoefficient(const float& coefficient) { if(!materialDataMapped_.empty()) materialDataMapped_[0]->environmentCoefficient = coefficient; }
	void SetTextureFilePath(const std::string filePath) { textureFilePath_ = filePath; }
	void SetTextureIndex(const uint32_t index) { textureIndex_ = index; }
};
