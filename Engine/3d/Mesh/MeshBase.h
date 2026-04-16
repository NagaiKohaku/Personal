#pragma once

#include "d3d12.h"

#include "Base/DirectXCommon.h"

#include <Math/Vector/Vector2.h>
#include <Math/Vector/Vector3.h>
#include <Math/Vector/Vector4.h>
#include <Math/Matrix/Matrix4x4.h>

#include "string"
#include "vector"
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

		void AddVertexData(const VertexData& vertex);

		///-------------------------------------------/// 
		/// ゲッター・セッター
		///-------------------------------------------///
	public:

		/// <summary>
		/// 頂点データを取得
		/// </summary>
		/// <returns>頂点データ</returns>
		const std::vector<VertexData>& GetVertexData() const { return vertexData_; }

		/// <summary>
		/// インデックスデータを取得
		/// </summary>
		/// <returns>インデックスデータ</returns>
		const std::vector<uint32_t>& GetIndexData() const { return indexData_; }

		///-------------------------------------------/// 
		/// メンバ変数
		///-------------------------------------------///
	protected:

		//DirectX基底
		DirectXCommon* dxCommon_ = nullptr;

		/// === バッファリソース === ///

		//頂点リソース
		Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_ = nullptr;

		//頂点番号リソース
		Microsoft::WRL::ComPtr<ID3D12Resource> indexResource_ = nullptr;

		/// === バッファリソース内のデータを指すポインタ === ///

		//頂点データ
		std::vector<VertexData> vertexData_;

		//頂点番号データ
		std::vector<uint32_t> indexData_;

		VertexData* mappedVertexData_;

		uint32_t* mappedIndexData_;

		/// === バッファビュー === ///

		//頂点バッファービュー
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

		//頂点番号バッファビュー
		D3D12_INDEX_BUFFER_VIEW indexBufferView_;

	};
}