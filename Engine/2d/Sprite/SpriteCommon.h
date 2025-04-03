#pragma once
#include "d3dx12.h"

#include "wrl.h"

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
	/// シングルトンインスタンス
	/// </summary>
	/// <returns>インスタンス</returns>
	static SpriteCommon* GetInstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	///-------------------------------------------/// 
	/// セッター・ゲッター
	///-------------------------------------------///
public:

	/// <summary>
	/// DirectX基底のゲッター
	/// </summary>
	/// <returns>DirectX基底</returns>
	DirectXCommon* GetDxCommon() const { return dxCommon_; }

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//DirectX基底
	DirectXCommon* dxCommon_;
};