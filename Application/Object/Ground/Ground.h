#pragma once

#include <Object/3D/Object3D.h>
#include <Scene/EngineContext.h>

#include "memory"

/// <summary>
/// 3D空間内の地面オブジェクトを管理するクラスです。
/// </summary>
class Ground {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// 地面オブジェクトを初期化します。
	/// </summary>
	void Initialize(EngineContext context);

	/// <summary>
	/// 地面オブジェクトの更新処理を行います。
	/// </summary>
	void Update();

	/// <summary>
	/// 地面オブジェクトの座標更新を行います。
	/// </summary>
	void TransformUpdate();

	/// <summary>
	/// 地面オブジェクトを描画します。
	/// </summary>
	void Draw();

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//地面オブジェクト
	std::unique_ptr<MyEngine::Object3D> object_;

	//UV座標Y
	float uvPosY;
};