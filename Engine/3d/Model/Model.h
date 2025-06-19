#pragma once

#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Matrix4x4.h"
#include "3d/Primitive/PrimitiveBase.h"
#include "3d/Primitive/PrimitiveType.h"

#include "DirectXTex.h"
#include "d3d12.h"

#include "string"
#include "vector"
#include "wrl.h"

class ModelCommon;

///=====================================================/// 
/// モデル
///=====================================================///
class Model {

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
	/// <param name="type"></param>
	/// <param name="textureFilePath"></param>
	void Initialize(PrimitiveType type,const std::string& textureFilePath);

	void Initialize(PrimitiveType type);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	void DrawPrimitive();

	void DrawMaterial();

	void DrawTexture();

	void Copy(Model* model);

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
		std::vector<PrimitiveBase::VertexData> vertices; // 頂点データ
		std::vector<uint32_t> indices; // インデックスデータ
	};

	///-------------------------------------------/// 
	/// ゲッター・セッター
	///-------------------------------------------///
public:

	/// <summary>
	/// 色のゲッター
	/// </summary>
	/// <returns></returns>
	const Vector4& GetColor() { return materialData_->color; }

	/// <summary>
	/// 光沢度のゲッター
	/// </summary>
	/// <returns>光沢度</returns>
	const float GetShininess() { return materialData_->shininess; }

	/// <summary>
	/// 色のセッター
	/// </summary>
	/// <param name="color">色</param>
	void SetColor(const Vector4& color) { materialData_->color = color; }

	/// <summary>
	/// 光沢度のセッター
	/// </summary>
	/// <param name="shininess">光沢度</param>
	void SetShininess(const float& shininess) { materialData_->shininess = shininess; }

	PrimitiveBase* GetPrimitive() { return primitive_.get(); }

	ModelData GetModelData() { return modelData_; }

	std::string GetTextureFilePath() const { return textureFilePath_; }

	void SetTextureFilePath(const std::string filePath) { textureFilePath_ = filePath; }

	void SetTextureIndex(const uint32_t index) { textureIndex_ = index; }

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//モデル基底
	ModelCommon* modelCommon_;

	//プリミティブ
	std::unique_ptr<PrimitiveBase> primitive_;

	//バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_ = nullptr;

	//バッファリソース内のデータを指すポインタ
	Material* materialData_ = nullptr;

	ModelData modelData_;

	std::string textureFilePath_;

	uint32_t textureIndex_;

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
	void LoadObjFile (const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// マテリアルデータの読み込み
	/// </summary>
	void LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);

};