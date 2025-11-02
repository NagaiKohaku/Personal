#pragma once

#include "memory"

#include "3d/Object/Object3D.h"

#include "Math/Vector3.h"

/// <summary>
/// 影クラス
/// </summary>
class Shadow {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="pos"></param>
	void Update(Vector3 pos);

	/// <summary>
	/// 描画
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