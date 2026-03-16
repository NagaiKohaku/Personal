#pragma once

#include <Object/Base/ObjectCommonBase.h>

#include <Math/Vector/Vector2.h>
#include <Math/Vector/Vector3.h>
#include <Math/Vector/Vector4.h>

namespace MyEngine {

	class Object2DCommon : public ObjectCommonBase {

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

	public:

		Object2DCommon();

		~Object2DCommon();

	public:

		void Initialize(DirectXCommon* dxCommonPtr) override;

		void CommonDrawSetting() override;

		void CreateRootSignature() override;

		void CreateInputElement(
			std::vector<D3D12_INPUT_ELEMENT_DESC>& inputElements
		) override;

		void CreateShader(
			Microsoft::WRL::ComPtr<IDxcBlob>& vs,
			Microsoft::WRL::ComPtr<IDxcBlob>& ps
		) override;

	private:

		/// === バッファリソース === ///

		//頂点リソース
		Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_ = nullptr;

		//頂点番号リソース
		Microsoft::WRL::ComPtr<ID3D12Resource> IndexResource_ = nullptr;

		/// === リソースデータ === ///

		//頂点データ
		VertexData* vertexData_ = nullptr;

		//頂点番号データ
		uint32_t* indexData_ = nullptr;

		/// === バッファビュー === ///

		//頂点バッファビュー
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

		//頂点番号バッファビュー
		D3D12_INDEX_BUFFER_VIEW indexBufferView_;

	public:

		const D3D12_VERTEX_BUFFER_VIEW& GetVertexBufferView() { return vertexBufferView_; }

		const D3D12_INDEX_BUFFER_VIEW& GetIndexBufferView() { return indexBufferView_; }

	};
}