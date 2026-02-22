#pragma once

#include "d3d12.h"

#include "Base/DirectXCommon.h"

#include <Math/Vector/Vector2.h>
#include <Math/Vector/Vector3.h>
#include <Math/Vector/Vector4.h>
#include <Math/Matrix/Matrix4x4.h>

#include "string"
#include "wrl.h"

namespace MyEngine {

	/// <summary>
	/// メッシュ基底クラス（Mesh Base）です。
	/// </summary>
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
		virtual void Initialize(DirectXCommon* dxCommonPtr) = 0;

		/// <summary>
		/// GPUにメッシュのデータを送信します。
		/// </summary>
		void SendDataForGPU();

		/// <summary>
		/// 外部から頂点データとインデックスデータをコピーしてメッシュを更新します。
		/// </summary>
		/// <param name="indices">コピーするインデックスデータの配列</param>
		/// <param name="vertices">コピーする頂点データの配列</param>
		void CopyMeshData(std::vector<uint32_t> indices, std::vector<VertexData> vertices);

		///-------------------------------------------/// 
		/// ゲッター・セッター
		///-------------------------------------------///
	public:

		/// <summary>
		/// 頂点数を取得
		/// </summary>
		/// <returns>頂点数</returns>
		uint32_t GetVertexCount() const { return vertexCount_; }

		/// <summary>
		/// インデックス数を取得
		/// </summary>
		/// <returns>インデックス数</returns>
		uint32_t GetIndexCount() const { return indexCount_; }

		/// <summary>
		/// 頂点データを取得
		/// </summary>
		/// <returns>頂点データ</returns>
		VertexData* GetVertexData() const { return vertexData_; }

		/// <summary>
		/// インデックスデータを取得
		/// </summary>
		/// <returns>インデックスデータ</returns>
		uint32_t* GetIndexData() const { return indexData_; }

		/// <summary>
		/// 頂点数を設定
		/// </summary>
		/// <param name="count">頂点数</param>
		void SetVertexCount(uint32_t count) { vertexCount_ = count; }

		/// <summary>
		/// インデックス数を設定
		/// </summary>
		/// <param name="count">インデックス数</param>
		void SetIndexCount(uint32_t count) { indexCount_ = count; }

		///-------------------------------------------/// 
		/// メンバ変数
		///-------------------------------------------///
	protected:

		//DirectX基底
		DirectXCommon* dxCommon_ = nullptr;

		//頂点数
		uint32_t vertexCount_ = 0;

		//インデックス数
		uint32_t indexCount_ = 0;

		/// === バッファリソース === ///

		//頂点リソース
		Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_ = nullptr;

		//頂点番号リソース
		Microsoft::WRL::ComPtr<ID3D12Resource> indexResource_ = nullptr;

		/// === バッファリソース内のデータを指すポインタ === ///

		//頂点データ
		VertexData* vertexData_ = nullptr;

		//頂点番号データ
		uint32_t* indexData_ = nullptr;

		/// === バッファビュー === ///

		//頂点バッファービュー
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

		//頂点番号バッファビュー
		D3D12_INDEX_BUFFER_VIEW indexBufferView_;

	};
}