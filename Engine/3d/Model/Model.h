#pragma once

#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Matrix4x4.h"

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
	/// 初期化処理
	/// </summary>
	/// <param name="directoryPath">ディレクトリパス</param>
	/// <param name="filename">ファイル名</param>
	void Initialize(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

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

	///-------------------------------------------/// 
	/// メンバ構造体
	///-------------------------------------------///
private:

	//頂点データ
	struct VertexData {
		Vector4  position;
		Vector2  texcoord;
		Vector3  normal;
	};

	//マテリアル
	struct Material {
		Vector4 color;
		int32_t enableLighting;
		float padding[3];
		Matrix4x4 uvTransform;
		float shininess;
	};

	//モデルのマテリアルデータ
	struct MaterialData {

		std::string textureFilePath;
		uint32_t textureIndex = 0;
	};

	//モデルデータ
	struct ModelData {

		std::vector<VertexData> vertices;
		std::vector<uint32_t> indexes;
		MaterialData material;
	};

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//モデル基底
	ModelCommon* modelCommon_;

	//モデルデータ
	ModelData modelData_;

	//バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource_ = nullptr;

	//バッファリソース内のデータを指すポインタ
	VertexData* vertexData_ = nullptr;
	Material* materialData_ = nullptr;
	uint32_t* indexData_ = nullptr;

	//バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	D3D12_INDEX_BUFFER_VIEW indexBufferView_;

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
	/// <param name="directoryPath">ディレクトリパス</param>
	/// <param name="filename">ファイル名</param>
	/// <returns>マテリアルデータ</returns>
	MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);

};