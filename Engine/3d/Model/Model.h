#pragma once

#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Matrix4x4.h"
#include "3d/Primitive/MeshBase.h"
#include "3d/Primitive/MeshType.h"

#include "DirectXTex.h"
#include "d3d12.h"

#include "string"
#include "vector"
#include "wrl.h"

/// === 前方宣言 === ///

class ModelCommon;

///=====================================================/// 
/// モデルクラス
///=====================================================///
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
	};

	struct ModelData {
		std::vector<MeshBase::VertexData> vertices; // 頂点データ
		std::vector<uint32_t> indices; // インデックスデータ
	};

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// 初期化処理(モデル)
	/// </summary>
	/// <param name="directoryPath">ディレクトリパス</param>
	/// <param name="filename">ファイル名</param>
	void Initialize(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// 初期化処理(プリミティブ)
	/// </summary>
	/// <param name="type">プリミティブタイプ</param>
	/// <param name="textureFilePath">テクスチャファイルパス</param>
	void Initialize(MeshType type, const std::string& textureFilePath);

	/// <summary>
	/// 初期化処理(コピー)
	/// </summary>
	/// <param name="type">プリミティブタイプ</param>
	/// <param name="model">モデル</param>
	void Initialize(MeshType type, Model* model);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// プリミティブの描画
	/// </summary>
	void DrawPrimitive();

	/// <summary>
	/// マテリアルの描画
	/// </summary>
	void DrawMaterial();

	/// <summary>
	/// テクスチャの描画
	/// </summary>
	void DrawTexture();

	/// <summary>
	/// モデルデータのコピー
	/// </summary>
	/// <param name="model">モデル</param>
	void Copy(Model* model);

	///-------------------------------------------/// 
	/// クラス内処理関数
	///-------------------------------------------///
private:

	/// <summary>
	/// objファイルの読み込み
	/// </summary>
	/// <param name="directoryPath">ディレクトリパス</param>
	/// <param name="filename">ファイル名</param>
	/// <returns>モデルデータ</returns>
	void LoadObjFile(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// マテリアルデータの読み込み
	/// </summary>
	void LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//モデル基底
	ModelCommon* modelCommon_;

	//プリミティブ
	std::unique_ptr<MeshBase> primitive_;

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
	/// プリミティブを取得
	/// </summary>
	/// <returns>プリミティブ</returns>
	MeshBase* GetPrimitive() { return primitive_.get(); }

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
	/// 光沢度を取得
	/// </summary>
	/// <returns>光沢度</returns>
	const float GetShininess() { return materialData_->shininess; }

	/// <summary>
	/// 色の設定
	/// </summary>
	/// <param name="color">色</param>
	void SetColor(const Vector4& color) { materialData_->color = color; }

	/// <summary>
	/// 光沢度の設定
	/// </summary>
	/// <param name="shininess">光沢度</param>
	void SetShininess(const float& shininess) { materialData_->shininess = shininess; }

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