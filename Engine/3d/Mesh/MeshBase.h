#pragma once

#include "d3d12.h"

#include "Base/DirectXCommon.h"

#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Matrix4x4.h"

#include <string>
#include <vector>
#include <wrl.h>

/// <summary>
/// メッシュ基底クラス（Mesh Base）です。
/// </summary>
/// <remarks>
/// - 頂点(VertexData)とインデックスデータを保持し、GPUバッファとバッファビューを管理します。
/// - 派生クラスで Initialize() を実装することで具体的な形状のメッシュを作成できます。
/// - Draw() で頂点・インデックスバッファを入力アセンブリにバインドし描画可能な状態にします。
/// - CopyMeshData() により外部データでメッシュの頂点・インデックス情報を更新可能です。
/// </remarks>
class MeshBase {

	///-------------------------------------------/// 
	/// 構造体
	///-------------------------------------------///
public:

	//頂点データ
	struct VertexData {
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// 純粋仮想デストラクタ
	/// </summary>
	virtual ~MeshBase() = default;

	/// <summary>
	/// メッシュを初期化する純粋仮想関数。
	/// </summary>
	/// <remarks>
	/// - 頂点・インデックスリソースの生成や初期データ設定などを行います。
	/// </remarks>
	virtual void Initialize();

	/// <summary>
	/// GPUにメッシュのデータを送信します。
	/// </summary>
	/// <remarks>
	/// - 頂点バッファとインデックスバッファを入力アセンブリステージにバインドします。
	/// </remarks>
	void SendDataForGPU();

	/// <summary>
	/// 外部から頂点データとインデックスデータをコピーしてメッシュを更新します。
	/// </summary>
	/// <param name="indices">コピーするインデックスデータの配列</param>
	/// <param name="vertices">コピーする頂点データの配列</param>
	/// <remarks>
	/// - 引数のデータを内部バッファ(vertexData_ / indexData_)に直接コピーします。
	/// </remarks>
	void CopyMeshData(std::vector<uint32_t> indices, std::vector<VertexData> vertices);

	/// <summary>
	/// GPU上のメッシュデータを更新します。
	/// </summary>
	void UpdateMeshDataGPU();

	/// <summary>
	/// 頂点データを追加します。
	/// </summary>
	/// <param name="vertex">頂点データ</param>
	void AddVertexData(const VertexData& vertex);

	///-------------------------------------------/// 
	/// ゲッター・セッター
	///-------------------------------------------///
public:

	/// <summary>
	/// 頂点データを取得します。
	/// </summary>
	/// <returns>頂点データ</returns>
	std::vector<VertexData>& GetVertexData() { return vertexData_; }

	/// <summary>
	/// インデックスデータを取得します。
	/// </summary>
	/// <returns>インデックスデータ</returns>
	std::vector<uint32_t>& GetIndexData() { return indexData_; }

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
protected:

	//DirectX基底
	DirectXCommon* directXCommon_ = nullptr;

	//頂点データ
	std::vector<VertexData> vertexData_;

	//インデックスデータ
	std::vector<uint32_t> indexData_;

	/// === バッファリソース === ///

	//頂点リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_ = nullptr;

	//頂点番号リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource_ = nullptr;

	/// === バッファリソース内のデータを指すポインタ === ///

	//頂点データ
	VertexData* mappedVertexData_ = nullptr;

	//頂点番号データ
	uint32_t* mappedIndexData_ = nullptr;

	/// === バッファビュー === ///

	//頂点バッファービュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

	//頂点番号バッファビュー
	D3D12_INDEX_BUFFER_VIEW indexBufferView_;

};

