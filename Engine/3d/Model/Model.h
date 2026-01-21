#pragma once

#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Matrix4x4.h"
#include "3d/Mesh/MeshBase.h"
#include "3d/Mesh/MeshType.h"
#include "3d/Model/ModelCommon.h"

#include "DirectXTex.h"
#include "d3d12.h"

#include "string"
#include "vector"
#include "wrl.h"

/// <summary>
/// 3Dモデル（Model）を管理するクラスです。
/// </summary>
class Model {

	///-------------------------------------------/// 
	/// メンバ構造体
	///-------------------------------------------///
private:

	//マテリアル
	struct Material {
		Vector4 color;
		int32_t enableLighting;
		float padding[3];
		Matrix4x4 uvTransform;
		float shininess;
		float environmentCoefficient;
	};

	//モデルデータ
	struct ModelData {
		std::vector<MeshBase::VertexData> vertices; // 頂点データ
		std::vector<uint32_t> indices; // インデックスデータ
	};

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// モデルを初期化します。(モデルデータ読み込み)
	/// </summary>
	/// <param name="directoryPath">モデルファイルのディレクトリパス</param>
	/// <param name="filename">モデルファイル名（OBJ 形式を想定）</param>
	void Initialize(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// モデルを初期化します。(メッシュクラスから生成)
	/// </summary>
	/// <param name="type">生成するメッシュの種類（PLANE, CUBE, SPHERE 等）</param>
	/// <param name="textureFilePath">テクスチャファイルのパス</param>
	void Initialize(MeshType type, const std::string& textureFilePath);

	/// <summary>
	/// モデルを初期化します。(既存モデルからコピー)
	/// </summary>
	/// <param name="type">生成するメッシュの種類（PLANE, CUBE, SPHERE 等）</param>
	/// <param name="model">コピー元のモデルインスタンス</param>
	void Initialize(MeshType type, Model* model);

	/// <summary>
	/// モデルを描画します。
	/// </summary>
	void Draw();

	/// <summary>
	/// モデルのメッシュデータを GPU に転送します。
	/// </summary>
	void SendMeshDataForGPU();

	/// <summary>
	/// モデルのマテリアルデータを GPU に転送します。
	/// </summary>
	void SendMaterialDataForGPU();

	/// <summary>
	/// モデルのテクスチャデータを GPU に転送します。
	/// </summary>
	void SendTextureDataForGPU();

	/// <summary>
	/// 指定したモデルのデータをコピーします。
	/// </summary>
	/// <param name="model">コピー元のモデル</param>
	void Copy(Model* model);

	///-------------------------------------------/// 
	/// クラス内処理関数
	///-------------------------------------------///
private:

	/// <summary>
	/// OBJファイルを読み込み、モデルデータに格納します。
	/// </summary>
	/// <param name="directoryPath">OBJファイルが存在するディレクトリパス</param>
	/// <param name="filename">OBJファイル名</param>
	void LoadObjFile(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// MTLファイルを読み込み、モデルのテクスチャパスを設定します。
	/// </summary>
	/// <param name="directoryPath">MTLファイルが存在するディレクトリパス</param>
	/// <param name="filename">MTLファイル名</param>
	void LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//モデル基底
	ModelCommon* modelCommon_;

	//メッシュ
	std::unique_ptr<MeshBase> mesh_;

	//マテリアルリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_ = nullptr;

	//マテリアルデータ
	Material* materialData_ = nullptr;

	//モデルデータ
	ModelData modelData_;

	//テクスチャファイルパス
	std::string textureFilePath_;

	//テクスチャ番号
	uint32_t textureIndex_;

	///-------------------------------------------/// 
	/// ゲッター・セッター
	///-------------------------------------------///
public:

	/// <summary>
	/// メッシュを取得
	/// </summary>
	/// <returns>メッシュ</returns>
	MeshBase* GetMesh() { return mesh_.get(); }

	/// <summary>
	/// モデルデータの取得
	/// </summary>
	/// <returns>モデルデータ</returns>
	ModelData GetModelData() { return modelData_; }

	/// <summary>
	/// テクスチャファイルパスを取得
	/// </summary>
	/// <returns>テクスチャファイルパス</returns>
	std::string GetTextureFilePath() const { return textureFilePath_; }

	/// <summary>
	/// 色を取得
	/// </summary>
	/// <returns>色</returns>
	const Vector4& GetColor() { return materialData_->color; }

	/// <summary>
	/// UV座標を取得
	/// </summary>
	/// <returns>UV座標</returns>
	const Matrix4x4& GetUVTransform() { return materialData_->uvTransform; }

	/// <summary>
	/// 光沢度を取得
	/// </summary>
	/// <returns>光沢度</returns>
	const float GetShininess() { return materialData_->shininess; }

	/// <summary>
	/// 環境反射係数を取得
	/// </summary>
	/// <returns>環境反射係数</returns>
	const float GetEnvironmentCoefficient() { return materialData_->environmentCoefficient; }

	/// <summary>
	/// 色の設定
	/// </summary>
	/// <param name="color">色</param>
	void SetColor(const Vector4& color) { materialData_->color = color; }

	/// <summary>
	/// UV座標を設定
	/// </summary>
	/// <param name="uvTransform"></param>
	void SetUVTransform(const Matrix4x4& uvTransform) { materialData_->uvTransform = uvTransform; }

	/// <summary>
	/// 光沢度の設定
	/// </summary>
	/// <param name="shininess">光沢度</param>
	void SetShininess(const float& shininess) { materialData_->shininess = shininess; }

	/// <summary>
	/// 環境反射係数を設定
	/// </summary>
	/// <param name="coefficient">環境反射係数</param>
	void SetEnvironmentCoefficient(const float& coefficient) { materialData_->environmentCoefficient = coefficient; }

	/// <summary>
	/// テクスチャファイルパスの設定
	/// </summary>
	/// <param name="filePath">テクスチャファイルパス</param>
	void SetTextureFilePath(const std::string filePath) { textureFilePath_ = filePath; }

	/// <summary>
	/// テクスチャ番号の設定
	/// </summary>
	/// <param name="index">テクスチャ番号</param>
	void SetTextureIndex(const uint32_t index) { textureIndex_ = index; }
};