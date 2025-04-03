#pragma once

#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Matrix4x4.h"
#include "Math/WorldTransform.h"

#include "d3d12.h"

#include "vector"
#include "wrl.h"

/// === 前方宣言 === ///
class DebugObjectCommon;

class Camera;

///=====================================================/// 
/// デバッグライン
///=====================================================///
class DebugLine {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	DebugLine();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="direction">ラインの方向</param>
	/// <param name="color">ラインの色</param>
	void Initialize(Vector3 direction,Vector4 color);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 親オブジェクトのセット
	/// </summary>
	void SetParent(WorldTransform* parent);

	/// <summary>
	/// 大きさのセット
	/// </summary>
	/// <param name="scale">大きさ</param>
	void SetScale(Vector3 scale);

	/// <summary>
	/// 回転角の設置
	/// </summary>
	/// <param name="rotate">回転角</param>
	void SetRotate(Vector3 rotate);

	/// <summary>
	/// 色のセット
	/// </summary>
	/// <param name="color">色</param>
	void SetColor(Vector4 color);

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
};