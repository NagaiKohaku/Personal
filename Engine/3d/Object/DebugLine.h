#pragma once

#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Matrix4x4.h"
#include "Math/WorldTransform.h"
#include "Base/LayerType.h"

#include "d3d12.h"

#include "vector"
#include "wrl.h"

/// === 前方宣言 === ///

class DebugObjectCommon;

class Camera;

/// <summary>
/// デバッグ用のラインを描画するクラス。
/// </summary>
/// <remarks>
/// - 主にシーン内のベクトル方向や衝突判定の可視化など、デバッグ目的で使用します。  
/// - 頂点／インデックスバッファ、マテリアル、座標変換行列などのリソースを内部で生成・管理します。  
/// </remarks>
class DebugLine {

	///-------------------------------------------/// 
	/// メンバ構造体
	///-------------------------------------------///
private:

	//頂点データ
	struct VertexData {
		Vector4 position;
	};

	//座標変換行列
	struct TransformationMatrix {
		Matrix4x4 WVP;
	};

	//マテリアル
	struct Material {
		Vector4 color;
	};

	struct ModelData {
		std::vector<VertexData> vertices;
		std::vector<uint32_t> indexes;
	};

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// 指定した方向にデバッグラインを初期化します。
	/// </summary>
	/// <param name="direction">ラインの方向ベクトル</param>
	/// <param name="color">ラインの描画色</param>
	/// <remarks>
	/// - この関数ではライン用の頂点・インデックスバッファ、マテリアル、変換行列リソースを生成します。  
	/// - 頂点は (0,0,0) を始点、方向ベクトルを終点として設定されます。  
	/// </remarks>
	void Initialize(Vector3 direction, Vector4 color);

	/// <summary>
	/// 指定した始点と終点の位置にデバッグラインを初期化します。
	/// </summary>
	/// <param name="start">ラインの始点</param>
	/// <param name="end">ラインの終点</param>
	/// <param name="color">ラインの描画色</param>
	/// <remarks>
	/// - この関数ではライン用の頂点・インデックスバッファ、マテリアル、変換行列リソースを生成します。  
	/// - 始点と終点の中点をラインの中心として設定します。
	/// </remarks>
	void Initialize(Vector3 start, Vector3 end, Vector4 color);

	/// <summary>
	/// デバッグラインのワールド変換および座標変換行列を更新します。
	/// </summary>
	/// <remarks>
	/// - ワールドトランスフォームを更新し、現在の位置・回転・スケールを反映します。
	/// - カメラが設定されている場合は、ワールドビュープロジェクション行列（WVP）を生成します。
	/// </remarks>
	void Update();

	/// <summary>
	/// デバッグラインを描画キューに登録します。
	/// </summary>
	/// <param name="layerType">描画レイヤーの種類</param>
	/// <remarks>
	/// - Renderクラスへ描画処理コマンドを登録します。  
	/// - コマンド内容は、頂点／インデックスバッファの設定、マテリアルおよび座標変換行列の送信、描画命令の発行です。  
	/// - layerTypeがDebugの場合はスワップチェーンへ直接描画し、それ以外の場合はオフスクリーンレンダーターゲットへ描画します。  
	/// </remarks>
	void Draw(LayerType layerType = Debug);

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//デバッグオブジェクト基底
	DebugObjectCommon* debugCommon_;

	//カメラ
	Camera* camera_;

	//ワールドトランスフォーム
	WorldTransform transform_;

	//バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResource_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_ = nullptr;

	//リソースデータ
	VertexData* vertexData_ = nullptr;
	uint32_t* indexData_ = nullptr;
	TransformationMatrix* wvpData_ = nullptr;
	Material* materialData_ = nullptr;

	//バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	D3D12_INDEX_BUFFER_VIEW indexBufferView_;

	//モデルデータ
	ModelData modelData_;

	///-------------------------------------------/// 
	/// ゲッター・セッター
	///-------------------------------------------///
public:

	/// <summary>
	/// ワールドトランスフォームの取得
	/// </summary>
	/// <returns>ワールドトランスフォーム</returns>
	WorldTransform& GetWorldTransform() { return transform_; }

	/// <summary>
	/// 親オブジェクトの設定
	/// </summary>
	/// <param name="parent">親オブジェクトのトランスフォーム</param>
	void SetParent(WorldTransform* parent);

	/// <summary>
	/// 大きさの設定
	/// </summary>
	/// <param name="scale">大きさ</param>
	void SetScale(Vector3 scale);

	/// <summary>
	/// 回転角の設定
	/// </summary>
	/// <param name="rotate">回転角</param>
	void SetRotate(Vector3 rotate);

	/// <summary>
	/// 色の設定
	/// </summary>
	/// <param name="color">色</param>
	void SetColor(Vector4 color);

};