#pragma once

#include "memory"

#include "3d/Object/Object3D.h"

#include <Math/Vector/Vector3.h>

/// <summary>
/// キャラクターや物体の位置に応じて影を生成・管理するクラス
/// </summary>
class Shadow {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// 影用オブジェクトを生成し初期設定を行う
	/// </summary>
	void Initialize();

	/// <summary>
	/// 指定座標に応じて影の位置とサイズを更新する
	/// </summary>
	/// <param name="pos">影の基準となる座標</param>
	void Update(Vector3 pos);

	/// <summary>
	/// 影オブジェクトを描画する
	/// </summary>
	void Draw();

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	// 影オブジェクト
	std::unique_ptr<Object3D> shadow_;

	// オブジェクト間の距離
	float maxLength_;

	// 最大サイズ
	float maxScale_;

	// 描画フラグ
	bool isDraw_;

};