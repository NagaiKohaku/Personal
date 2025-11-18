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

/// <summary>
/// 3D空間の背景として表示されるスカイボックスを管理・描画するクラスです。
/// </summary>
/// <remarks>
/// - ゲームやシーンの背景として、遠景に広がる空や空間を立方体の内側に描画します。  
/// </remarks>
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
	/// スカイボックスを初期化します。
	/// </summary>
	/// <remarks>
	/// - スカイボックス共通クラスのシングルトンインスタンスを取得し、必要なリソースの生成を行います。  
	/// - スカイボックスは6面体で構成されるため、頂点とインデックスを6面分用意します。  
	/// - 頂点リソースを作成し、GPUに送信できるようマッピングします。  
	/// - マテリアルリソースを生成し、基本色やライティング有効設定、光沢度、UV変換行列などを初期化します。  
	/// - 指定されたファイルパスからキューブテクスチャを読み込み、スカイボックス全体に適用します。  
	/// - WVP行列用のリソースを生成し、初期化した変換データを設定します。  
	/// </remarks>
	void Initialize(std::string filePath);

	/// <summary>
	/// スカイボックスの更新処理を行います。
	/// </summary>
	/// <remarks>
	/// - トランスフォームの行列を更新し、スカイボックスの現在の位置・回転・スケールを反映します。  
	/// - ワールド行列を基にWVP行列を作成します。  
	/// - 計算結果をGPU用の座標変換行列リソースに書き込みます。  
	/// </remarks>
	void Update();

	/// <summary>
	/// スカイボックスの描画処理を行います。
	/// </summary>
	/// <remarks>
	/// - スカイボックスを描画するためのコマンドを生成し、レンダラーに登録します。  
	/// - コマンド内では、描画に必要な共通設定を適用し、頂点・インデックス・マテリアル・テクスチャ・変換行列などをGPUに転送します。  
	/// - 最後に描画命令を発行し、背景層としてスカイボックスを描画できるようにします。  
	/// </remarks>
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

	//頂点数
	uint32_t vertexCount_;

	//頂点番号数
	uint32_t indexCount_;

	//面数
	uint32_t surfaceCount_;

	//テクスチャファイルパス
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

	/// === リソースデータ === ///

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