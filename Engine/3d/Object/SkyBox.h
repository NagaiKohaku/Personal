#pragma once

#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Matrix4x4.h"
#include "Math/WorldTransform.h"

#include "d3d12.h"
#include "wrl.h"
#include "stdint.h"
#include "string"

/// === 前方宣言 === ///

class SkyBoxCommon;

class Camera;

///=====================================================/// 
/// SkyBoxクラス
///=====================================================///
class SkyBox {

	///-------------------------------------------/// 
	/// メンバ構造体
	///-------------------------------------------///
private:

	//頂点データ
	struct VertexData {
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};

	//座標変換行列データ
	struct TransformationMatrix {
		Matrix4x4 WVP;
		Matrix4x4 World;
		Matrix4x4 WorldInverseTranspose;
	};

	//マテリアル
	struct Material {
		Vector4 color;
		int32_t enableLighting;
		float padding[3];
		Matrix4x4 uvTransform;
		float shininess;
	};

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize(std::string filePath);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//SkyBox基底
	SkyBoxCommon* skyBoxCommon_ = nullptr;

	//カメラ
	Camera* camera_;

	//座標データ
	WorldTransform transform_;

	uint32_t vertexCount_ = 0;

	uint32_t indexCount_ = 0;

	uint32_t surfaceCount_ = 6; // SkyBoxは6面体

	std::string textureFilePath_;

	/// === バッファリソース === ///

	//頂点リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_ = nullptr;

	//頂点番号リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> IndexResource_ = nullptr;

	//マテリアルリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_ = nullptr;

	//座標変換行列リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> WVPResource_ = nullptr;

	/// === バッファリソース内のデータを指すポインタ === ///

	//頂点データ
	VertexData* vertexData_ = nullptr;

	//頂点番号データ
	uint32_t* indexData_ = nullptr;

	//マテリアルデータ
	Material* materialData_ = nullptr;

	//座標変換行列データ
	TransformationMatrix* WVPData_ = nullptr;

	/// === バッファビュー === ///

	//頂点バッファービュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

	//頂点番号バッファビュー
	D3D12_INDEX_BUFFER_VIEW indexBufferView_;

	///-------------------------------------------/// 
	/// ゲッター・セッター
	///-------------------------------------------///
public:

	/// <summary>
	/// ワールドトランスフォームを取得
	/// </summary>
	/// <returns>ワールドトランスフォーム</returns>
	WorldTransform& GetWorldTransform() { return transform_; }

	/// <summary>
	/// カメラの設定
	/// </summary>
	/// <param name="camera">カメラ</param>
	void SetCamera(Camera* camera) { camera_ = camera; }

	/// <summary>
	/// テクスチャファイルパスの設定
	/// </summary>
	/// <returns>ファイルパス</returns>
	std::string GetTextureFilePath() const { return textureFilePath_; }

};