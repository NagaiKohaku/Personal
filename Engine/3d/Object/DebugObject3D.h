#pragma once

#include "3d/Model/Model.h"
#include <Base/Renderer.h>

#include <Math/Matrix/Matrix4x4.h>
#include <Math/Transform/WorldTransform.h>

#include "wrl.h"

namespace MyEngine {

	/// === 前方宣言 === ///

	class DebugObjectCommon;

	class Camera;

	/// <summary>
	/// 3D空間上のデバッグ用オブジェクトを管理・描画するクラスです。
	/// </summary>
	class DebugObject3D {

		///-------------------------------------------/// 
		/// メンバ構造体
		///-------------------------------------------///
	private:

		//座標変換行列データ
		struct TransformationMatrix {
			Matrix4x4 WVP;
			Matrix4x4 World;
			Matrix4x4 WorldInverseTranspose;
		};

		///-------------------------------------------/// 
		/// メンバ関数
		///-------------------------------------------///
	public:

		/// <summary>
		/// 3Dデバッグオブジェクトを初期化します。
		/// </summary>
		void Initialize(DebugObjectCommon* debugObjectCommonPtr, Renderer* rendererPtr);

		/// <summary>
		/// 3Dデバッグオブジェクトのトランスフォームを更新します。
		/// </summary>
		void Update();

		/// <summary>
		/// 3Dデバッグオブジェクトを描画します。
		/// </summary>
		void Draw();

		///-------------------------------------------/// 
		/// メンバ変数
		///-------------------------------------------///
	private:

		//デバッグオブジェクト基底
		DebugObjectCommon* debugCommon_;

		//カメラ
		Camera* camera_;

		Renderer* renderer_;

		//座標データ
		WorldTransform transform_;

		//バッファリソース
		Microsoft::WRL::ComPtr<ID3D12Resource> WVPResource_ = nullptr;

		//バッファリソース内のデータを指すポインタ
		TransformationMatrix* WVPData_ = nullptr;

		//モデル情報
		std::unique_ptr<Model> model_;

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
		/// モデルを取得
		/// </summary>
		/// <returns>モデル</returns>
		Model* GetModel() const { return model_.get(); }

		/// <summary>
		/// カメラの設定
		/// </summary>
		/// <param name="camera">カメラ</param>
		void SetCamera(Camera* camera) { camera_ = camera; }

		/// <summary>
		/// モデルの設定
		/// </summary>
		/// <param name="modelName">ファイル名</param>
		void SetModel(const std::string& modelName);
	};
}