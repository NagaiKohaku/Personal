#pragma once

#include "2d/Object/Object2D.h"
#include "3d/Object/Object3D.h"

#include "memory"

/// === 前方宣言 === ///

class Camera;

class Player;

///=====================================================/// 
/// レティクルクラス
///=====================================================///
class Reticle {

public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="cameraPtr">カメラポインタ</param>
	/// <param name="playerPtr">プレイヤーポインタ</param>
	void Initialize(Camera* cameraPtr,Player* playerPtr);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

public:

	/// <summary>
	/// ワールド座標のゲッター
	/// </summary>
	/// <returns>座標</returns>
	Vector3 GetWorldPos() const { return object3D_->GetWorldTransform().GetWorldTranslate(); }

private:

	//カメラ
	Camera* camera_;

	//プレイヤー
	Player* player_;

	//2Dレティクル
	std::unique_ptr<Object2D> object2D_;

	//3Dレティクル
	std::unique_ptr<Object3D> object3D_;

	//オフセット
	Vector3 offset_;

	//移動範囲
	Vector3 moveRange_;

	//移動強度
	float moveStrength_;

	//2Dレティクルの描画フラグ
	bool isDraw2D_;

	//3Dレティクルの描画フラグ
	bool isDraw3D_;
};