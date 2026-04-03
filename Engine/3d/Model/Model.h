#pragma once

#include <Math/Vector/Vector2.h>
#include <Math/Vector/Vector3.h>
#include <Math/Vector/Vector4.h>
#include <Math/Matrix/Matrix4x4.h>
#include "3d/Mesh/MeshBase.h"
#include "3d/Mesh/MeshType.h"
#include "3d/Model/ModelCommon.h"

#include "DirectXTex.h"
#include "d3d12.h"

#include "string"
#include "vector"
#include "wrl.h"

namespace MyEngine {

	/// <summary>
	/// 3Dモデル（Model）を管理するクラスです。
	/// </summary>
	class Model {

		///-------------------------------------------/// 
		/// メンバ構造体
		///-------------------------------------------///
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

		//モデルデータ
		struct ModelData {
			std::vector<MeshBase::VertexData> vertices; // 頂点データ
			std::vector<uint32_t> indices; // インデックスデータ
		};

		//モデルの部位データ(マルチメッシュ用)
		struct MeshPart {
			std::string materialName;
			std::unique_ptr<MeshBase> mesh;
			Microsoft::WRL::ComPtr<ID3D12Resource> materialResource = nullptr;
			Material* materialData = nullptr;
			ModelData modelData;
			std::string textureFilePath;
			uint32_t textureIndex;
		};

	private:



		///-------------------------------------------/// 
		/// メンバ関数
		///-------------------------------------------///
	public:

		/// <summary>
		/// モデルを初期化します。(モデルデータ読み込み)
		/// </summary>
		/// <param name="directoryPath">モデルファイルのディレクトリパス</param>
		/// <param name="filename">モデルファイル名（OBJ 形式を想定）</param>
		void Initialize(const std::string& directoryPath, const std::string& filename, ModelCommon* modelCommonPtr);

		/// <summary>
		/// モデルを初期化します。(メッシュクラスから生成)
		/// </summary>
		/// <param name="type">生成するメッシュの種類（PLANE, CUBE, SPHERE 等）</param>
		/// <param name="textureFilePath">テクスチャファイルのパス</param>
		void Initialize(MeshType type, const std::string& textureFilePath, ModelCommon* modelCommonPtr);

		/// <summary>
		/// モデルを初期化します。(既存モデルからコピー)
		/// </summary>
		/// <param name="type">生成するメッシュの種類（PLANE, CUBE, SPHERE 等）</param>
		/// <param name="model">コピー元のモデルインスタンス</param>
		void Initialize(MeshType type, Model* model, ModelCommon* modelCommonPtr);

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
		/// MTLファイルを読み込み、マテリアル名とテクスチャパスのマップを生成します。
		/// </summary>
		/// <param name="directoryPath">MTLファイルが存在するディレクトリパス</param>
		/// <param name="filename">MTLファイル名</param>
		/// <returns>マテリアル名とテクスチャパスのマップ</returns>
		std::unordered_map<std::string, std::string> LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);

		///-------------------------------------------/// 
		/// メンバ変数
		///-------------------------------------------///
	private:

		//モデル基底
		ModelCommon* modelCommon_;

		//メッシュパーツのリスト（マルチメッシュ対応）
		std::vector<MeshPart> meshParts_;

		///-------------------------------------------/// 
		/// ゲッター・セッター
		///-------------------------------------------///
	public:

		/// <summary>
		/// メッシュパーツのリストを取得
		/// </summary>
		/// <returns>メッシュパーツのリスト</returns>
		std::vector<MeshPart>& GetMeshParts() { return meshParts_; }

		/// <summary>
		/// メッシュを取得
		/// </summary>
		/// <param name="index">パーツのインデックス番号(デフォルトは0)</param>
		/// <returns>メッシュ</returns>
		MeshBase* GetMesh(size_t index = 0) { return meshParts_[index].mesh.get(); }

		/// <summary>
		/// モデルデータの取得
		/// </summary>
		/// <param name="index">パーツのインデックス番号(デフォルトは0)</param>
		/// <returns>モデルデータ</returns>
		ModelData GetModelData(size_t index = 0) { return meshParts_[index].modelData; }

		/// <summary>
		/// テクスチャファイルパスを取得
		/// </summary>
		/// <param name="index">パーツのインデックス番号(デフォルトは0)</param>
		/// <returns>テクスチャファイルパス</returns>
		std::string GetTextureFilePath(size_t index = 0) const { return meshParts_[index].textureFilePath; }

		/// <summary>
		/// 色を取得
		/// </summary>
		/// <param name="index">パーツのインデックス番号(デフォルトは0)</param>
		/// <returns>色</returns>
		const Vector4& GetColor(size_t index = 0) { return meshParts_[index].materialData->color; }

		/// <summary>
		/// UV座標を取得
		/// </summary>
		/// <param name="index">パーツのインデックス番号(デフォルトは0)</param>
		/// <returns>UV座標</returns>
		const Matrix4x4& GetUVTransform(size_t index = 0) { return meshParts_[index].materialData->uvTransform; }

		/// <summary>
		/// 光沢度を取得
		/// </summary>
		/// <param name="index">パーツのインデックス番号(デフォルトは0)</param>
		/// <returns>光沢度</returns>
		const float GetShininess(size_t index = 0) { return meshParts_[index].materialData->shininess; }

		/// <summary>
		/// 環境反射係数を取得
		/// </summary>
		/// <param name="index">パーツのインデックス番号(デフォルトは0)</param>
		/// <returns>環境反射係数</returns>
		const float GetEnvironmentCoefficient(size_t index = 0) { return meshParts_[index].materialData->environmentCoefficient; }

		/// <summary>
		/// 全パーツに色を設定
		/// </summary>
		/// <param name="color">色</param>
		void SetColor(const Vector4& color) {
			for (auto& part : meshParts_) {
				part.materialData->color = color;
			}
		}

		/// <summary>
		/// 全パーツにUV座標を設定
		/// </summary>
		/// <param name="uvTransform">UV Transform</param>
		void SetUVTransform(const Matrix4x4& uvTransform) {
			for (auto& part : meshParts_) {
				part.materialData->uvTransform = uvTransform;
			}
		}

		/// <summary>
		/// 全パーツに光沢度を設定
		/// </summary>
		/// <param name="shininess">光沢度</param>
		void SetShininess(const float& shininess) {
			for (auto& part : meshParts_) {
				part.materialData->shininess = shininess;
			}
		}

		/// <summary>
		/// 全パーツに環境反射係数を設定
		/// </summary>
		/// <param name="coefficient">環境反射係数</param>
		void SetEnvironmentCoefficient(const float& coefficient) {
			for (auto& part : meshParts_) {
				part.materialData->environmentCoefficient = coefficient;
			}
		}

		/// <summary>
		/// 全パーツにテクスチャファイルパスを設定
		/// </summary>
		/// <param name="filePath">テクスチャファイルパス</param>
		void SetTextureFilePath(const std::string filePath) {
			for (auto& part : meshParts_) {
				part.textureFilePath = filePath;
			}
		}

		/// <summary>
		/// 全パーツにテクスチャ番号を設定
		/// </summary>
		/// <param name="index">テクスチャ番号</param>
		void SetTextureIndex(const uint32_t index) {
			for (auto& part : meshParts_) {
				part.textureIndex = index;
			}
		}
	};
}