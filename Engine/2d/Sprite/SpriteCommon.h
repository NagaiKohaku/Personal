#pragma once

#include "Base/DirectXCommon.h"

#include "d3dx12.h"

#include "wrl.h"

/// <summary>
/// スプライト描画共通処理を管理するシングルトンクラス。
/// </summary>
class SpriteCommon {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// SpriteCommonのシングルトンインスタンスを取得します。
	/// </summary>
	static SpriteCommon* GetInstance();

	/// <summary>
	/// SpriteCommonを初期化します。
	/// </summary>
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