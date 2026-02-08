#pragma once

#include "Base/DirectXCommon.h"

namespace MyEngine {

	/// <summary>
	/// モデル共通処理を管理するシングルトンクラスです。
	/// </summary>
	class ModelCommon {

		///-------------------------------------------/// 
		/// メンバ関数
		///-------------------------------------------///
	public:

		/// <summary>
		/// ModelCommonのシングルトンインスタンスを取得します。
		/// </summary>
		static ModelCommon* GetInstance();

		/// <summary>
		/// ModelCommonの初期化を行います。
		/// </summary>
		void Initialize();

		///-------------------------------------------/// 
		/// メンバ変数
		///-------------------------------------------///
	private:

		//DirectX基底
		DirectXCommon* dxCommon_;

		///-------------------------------------------/// 
		/// ゲッター・セッター
		///-------------------------------------------///
	public:

		/// <summary>
		/// DirectX基底の取得
		/// </summary>
		/// <returns>DirectX基底</returns>
		DirectXCommon* GetDxCommon() const { return dxCommon_; }

	};
}