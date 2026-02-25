#include "Reticle.h"

#include "Base/WinApp.h"
#include "3d/Camera/Camera.h"
#include "2d/Sprite/SpriteManager.h"
#include <Math/Utility/MakeMatrixMath.h>
#include <Math/Utility/Easing.h>

#include "algorithm"

using namespace MyEngine;

///=====================================================/// 
/// レティクルを初期化
///=====================================================///
void Reticle::Initialize(Object2DCommon* object2DCommonPtr, Object3DCommon* object3DCommonPtr, DebugObjectCommon* debugObjectCommonPtr, Camera* cameraPtr) {

	//カメラポインタを取得
	camera_ = cameraPtr;

	/// === 2Dオブジェクトの生成 === ///

	//生成
	object2D_ = std::make_unique<Object2D>();

	//初期化
	object2D_->Initialize(object2DCommonPtr, camera_);

	//スプライトをセット
	object2D_->SetSprite("Reticle");

	//サイズを小さくする
	object2D_->SetSize(object2D_->GetSize() * 0.25f);

	//スプライトのアンカーポイントを中心に設定
	object2D_->GetSprite()->SetAnchorPoint({ 0.5f,0.5f });

	/// === 3Dオブジェクトの生成 === ///

	//生成
	object3D_ = std::make_unique<Object3D>();

	//初期化
	object3D_->Initialize(object3DCommonPtr, debugObjectCommonPtr);

	//モデルを設定
	object3D_->SetModel("Cube");

	/// === 他変数の設定 === ///

	//2Dレティクルの描画フラグを設定
	isDraw2D_ = true;

	//3Dレティクルの描画フラグを設定
	isDraw3D_ = false;

	//アニメーションタイマーを初期化
	animTimer_ = 0.0f;

	//アニメーション最大時間を設定
	animMaxTime_ = 0.5f;
}

///=====================================================/// 
/// レティクルの更新処理
///=====================================================///
void Reticle::Update() {

	/// === 起動アニメーションの更新 === ///

	if( animTimer_ <= animMaxTime_) {

		animTimer_ += 1.0f / 60.0f;
	} else {

		animTimer_ = animMaxTime_;
	}

	Vector4 color = Lerp(Vector4(0.0f, 1.0f, 1.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), animTimer_ / animMaxTime_ );

	object2D_->GetSprite()->SetColor(color);

	/// === 3Dオブジェクトの移動 === ///

	//3Dオブジェクトの更新
	object3D_->Update();

	/// === 2Dオブジェクトの移動 === ///

	//3Dオブジェクトの座標をスクリーン座標に変換する
	Vector3 screenPos = Vector3ToScreenSpace(camera_,object3D_->GetWorldTransform().translate_);

	//スクリーン座標を設定する
	object2D_->SetTranslate({ screenPos.x,screenPos.y });

	//2Dオブジェクトの更新
	object2D_->Update();
}

///=====================================================/// 
/// レティクルの描画処理
///=====================================================///
void Reticle::Draw() {

	if (isDraw2D_) {

		//2Dオブジェクトの描画
		object2D_->Draw(LayerType::UI);
	}

	if (isDraw3D_) {

		//3Dオブジェクトの描画
		object3D_->Draw(LayerType::OBJECT);
	}
}