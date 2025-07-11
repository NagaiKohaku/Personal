#pragma once
#include "d3dx12.h"

#include "wrl.h"

/// === 前方宣言 === ///

class DirectXCommon;

///=====================================================/// 
/// スプライト基底
///=====================================================///
class SpriteCommon {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// シングルトンインスタンスを取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static SpriteCommon* GetInstance();

	/// <summary>
	/// 初期化
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