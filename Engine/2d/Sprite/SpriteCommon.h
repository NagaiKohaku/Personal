#pragma once

#include "Base/DirectXCommon.h"

#include "d3dx12.h"

#include "wrl.h"

/// <summary>
/// スプライト描画共通処理を管理するシングルトンクラス。
/// </summary>
/// <remarks>
/// - DirectXCommonのインスタンスを保持
/// </remarks>
class SpriteCommon {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// SpriteCommonのシングルトンインスタンスを取得します。
	/// </summary>
	/// <remarks>
	/// 返り値に静的インスタンスを返します。
	/// </remarks>
	static SpriteCommon* GetInstance();

	/// <summary>
	/// SpriteCommonを初期化します。
	/// </summary>
	/// <remarks>
	/// DirectXCommonのシングルトンインスタンスを取得し、内部メンバ(dxCommon_)に保持します。
	/// </remarks>
	void Initialize();

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//DirectX基底
	DirectXCommon* dxCommon_;

	///-------------------------------------------/// 
	/// セッター・ゲッター
	///-------------------------------------------///
public:

	/// <summary>
	/// DirectX基底を取得
	/// </summary>
	/// <returns>DirectX基底</returns>
	DirectXCommon* GetDxCommon() const { return dxCommon_; }
};