#pragma once

#include "d3d12.h"

#include "Base/DirectXCommon.h"

#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Matrix4x4.h"

#include "string"
#include "wrl.h"

class PrimitiveBase {

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~PrimitiveBase() = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() = 0;

public:

	uint32_t GetIndexCount() const { return indexCount_; }

protected:

	//頂点データ
	struct VertexData {
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};

protected:

	DirectXCommon* directXCommon_ = nullptr;

	uint32_t indexCount_ = 0;

	/// === バッファリソース === ///

	//頂点リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_ = nullptr;

	//頂点番号リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> IndexResource_ = nullptr;

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

