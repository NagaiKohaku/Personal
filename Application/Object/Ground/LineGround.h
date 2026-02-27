#pragma once

#include "3d/Object/DebugLine.h"
#include <Base/Renderer.h>

#include "memory"
#include "list"

/// <summary>
/// ライン状の床オブジェクトを管理するクラスです。
/// </summary>
class LineGround {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// ライン状の床オブジェクトを初期化します。
	/// </summary>
	void Initialize(MyEngine::DebugObjectCommon* debugObjectCommonPtr, MyEngine::Renderer* rendererPtr);

	/// <summary>
	/// ライン状の床オブジェクトの更新処理を行います。
	/// </summary>
	void Update();

	/// <summary>
	/// ライン状の床オブジェクトを描画します。
	/// </summary>
	void Draw();

	///-------------------------------------------/// 
	/// クラス内処理関数
	///-------------------------------------------///
private:

	/// <summary>
	/// ライン状の床オブジェクトの横ラインを移動させます。
	/// </summary>
	void Move();

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//縦ライン群
	std::list<std::unique_ptr<MyEngine::DebugLine>> verticalLines_;

	//横ライン群
	std::list<std::unique_ptr<MyEngine::DebugLine>> horizontalLines_;

	//ライン分割数
	float lineDivide_ = 30.0f;

	//ライン間隔
	float lineDistance_ = 5.0f;

	//移動速度
	float speed_;
};