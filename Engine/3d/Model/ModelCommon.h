#pragma once

#include "Base/DirectXCommon.h"

/// <summary>
/// モデル共通処理を管理するシングルトンクラスです。
/// </summary>
/// <remarks>
/// - DirectX共通基底(DirectXCommon)へのアクセスを提供します。
/// - モデル生成や描画処理で必要となる共通リソースを保持します。
/// </remarks>
class ModelCommon {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// ModelCommonのシングルトンインスタンスを取得します。
	/// </summary>
	/// <remarks>
	/// 返り値に静的インスタンスを返します。
	/// </remarks>
	static ModelCommon* GetInstance();

	/// <summary>
	/// ModelCommonの初期化を行います。
	/// </summary>
	/// <remarks>
	/// - DirectXCommonのインスタンスを取得
	/// </remarks>
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