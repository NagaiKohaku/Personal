#pragma once

#include <Math/Vector/Vector2.h>
#include <Math/Vector/Vector3.h>
#include <Math/Vector/Vector4.h>
#include <Math/Matrix/Matrix4x4.h>
#include <Math/Transform/WorldTransform.h>
#include "Base/LayerType.h"

#include "d3d12.h"

#include "vector"
#include "wrl.h"

namespace MyEngine {

	/// === 前方宣言 === ///

	class DebugObjectCommon;

	class Camera;

	/// <summary>
	/// デバッグ用のラインを描画するクラス。
	/// </summary>
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
		void Initialize(Vector3 direction, Vector4 color);

		/// <summary>
		/// 指定した始点と終点の位置にデバッグラインを初期化します。
		/// </summary>
		/// <param name="start">ラインの始点</param>
		/// <param name="end">ラインの終点</param>
		/// <param name="color">ラインの描画色</param>
		void Initialize(Vector3 start, Vector3 end, Vector4 color);

		/// <summary>
		/// デバッグラインのワールド変換および座標変換行列を更新します。
		/// </summary>
		void Update();

		/// <summary>
		/// デバッグラインを描画キューに登録します。
		/// </summary>
		/// <param name="layerType">描画レイヤーの種類</param>
		void Draw(LayerType layerType = LayerType::DEBUG);

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
}