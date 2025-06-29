#include "Reticle.h"

#include "Base/WinApp.h"
#include "3d/Camera/Camera.h"
#include "Player/Player.h"
#include "2d/Sprite/SpriteManager.h"
#include "Math/MakeMatrixMath.h"
#include "Math/Easing.h"

#include "algorithm"

///=====================================================/// 
/// 初期化
///=====================================================///
void Reticle::Initialize(Camera* cameraPtr, Player* playerPtr) {

	//カメラポインタを取得
	camera_ = cameraPtr;

	//プレイヤーポインタを取得
	player_ = playerPtr;

	//ReticleSpriteを読み込む
	SpriteManager::GetInstance()->LoadSprite("Reticle", "Reticle");

	/// === 2Dオブジェクトの生成 === ///

	//生成
	object2D_ = std::make_unique<Object2D>();

	//初期化
	object2D_->Initialize();

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
	object3D_->Initialize();

	//モデルを設定
	object3D_->SetModel("Cube");

	/// === 他変数の設定 === ///

	//プレイヤーからのオフセットを設定
	offset_ = { 0.0f,0.0f,30.0f };

	//移動範囲を設定
	moveRange_ = { 7.0f,5.0f,0.0f };

	//移動強度を設定
	moveStrength_ = 5.0f;

	//2Dレティクルの描画フラグを設定
	isDraw2D_ = true;

	//3Dレティクルの描画フラグを設定
	isDraw3D_ = false;

	//初期座標を設定
	object3D_->GetWorldTransform().translate_ = player_->GetWorldPos() + offset_;

}

///=====================================================/// 
/// 更新
///=====================================================///
void Reticle::Update() {

	/// === 3Dオブジェクトの移動 === ///

	//プレイヤーの座標
	Vector3 playerPos = player_->GetWorldPos();

	//プレイヤーの移動方向
	Vector3 moveDirection = Normalize(player_->GetVelocity());

	//プレイヤーが動いていれば
	if (Length(moveDirection) != 0.0f) {

		//プレイヤーの現在位置からオフセット分移動した座標
		Vector3 offsetPos = player_->GetWorldPos() + offset_;

		//移動後の座標
		Vector3 movePos = offsetPos + (moveDirection * Length(moveRange_));

		//制限範囲からでないようにする
		movePos = {
			std::clamp(movePos.x, playerPos.x - moveRange_.x, playerPos.x + moveRange_.x),
			std::clamp(movePos.y, playerPos.y - moveRange_.y, playerPos.y + moveRange_.y),
			movePos.z,
		};

		//線形補間で移動
		object3D_->GetWorldTransform().translate_ = Lerp(object3D_->GetWorldTransform().translate_, movePos, moveStrength_ / 100.0f);
	}

	//地面の中に埋まらないようにする
	object3D_->GetWorldTransform().translate_.y = fmaxf(1.0f, object3D_->GetWorldTransform().translate_.y);

	//3Dオブジェクトの更新
	object3D_->Update();

	/// === 2Dオブジェクトの移動 === ///

	//ビューポート行列
	Matrix4x4 viewport = MakeViewportMatrix(0, 0, WinApp::kClientWidth, WinApp::kClientHeight, 0, 1);

	//カメラのビュープロジェクション行列とビューポート行列を掛ける
	Matrix4x4 viewProjectionViewport = camera_->GetViewProjectionMatrix() * viewport;

	//3Dオブジェクトの座標をスクリーン座標に変換する
	Vector3 screenPos = Transform(object3D_->GetWorldTransform().translate_, viewProjectionViewport);

	//スクリーン座標を設定する
	object2D_->SetTranslate({ screenPos.x,screenPos.y });

	//2Dオブジェクトの更新
	object2D_->Update();
}

///=====================================================/// 
/// 描画
///=====================================================///
void Reticle::Draw() {

	if (isDraw2D_) {

		//2Dオブジェクトの描画
		object2D_->Draw(LayerType::UI);
	}

	if (isDraw3D_) {

		//3Dオブジェクトの描画
		object3D_->Draw(LayerType::Object);
	}
}