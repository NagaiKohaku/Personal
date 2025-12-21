#include "GameScene3DObject.h"

#include <Shake/Shake.h>
#include <Fade/Fade.h>
#include <ObjectManager.h>
#include <Math/Easing.h>

#include <numbers>

void GameScene3DObject::Initialize() {

	/// === カメラの設定 === ///

	camera_ = std::make_unique<Camera>();

	camera_->Initialize();

	//デバッグカメラを使用しない
	camera_->SetDebugCameraFlag(false);

	//カメラの座標
	camera_->GetWorldTransform().translate_ = { 0.0f,3.0f,0.0f };

	//シェイクにカメラをセット
	Shake::GetInstance()->SetCamera(camera_.get());

	/// === エネミーマネージャーの生成 === ///

	enemyManager_ = std::make_unique<EnemyManager>();

	/// === バレットマネージャーの生成 === ///

	bulletManager_ = std::make_unique<BulletManager>();

	bulletManager_->Initialize();

	/// === グラウンドマネージャーの生成 === ///

	groundManager_ = std::make_unique<GroundManager>();

	groundManager_->Initialize();

	/// === プレイヤーの生成 === ///

	//オブジェクトマネージャーで生成
	ObjectManager::GetInstance()->SpawnPlayer();

	//オブジェクトマネージャーからポインタを受け取る
	player_ = ObjectManager::GetInstance()->GetPlayer();

	//初期化
	player_->Initialize(camera_.get(), bulletManager_.get(), false);

	//最初は動けないようにする
	player_->SetIsMoveActive(false);

	//エネミーマネージャーの初期化
	enemyManager_->Initialize(camera_.get(), bulletManager_.get(), player_);

	/// === 追尾カメラの生成 === ///

	followCamera_ = std::make_unique<FollowCamera>();

	followCamera_->Initialize(camera_.get(), player_);

	//最初は無効化する
	followCamera_->SetIsActive(false);

	/// === エミッターの生成 === ///

	//衝撃波エミッター(左)
	shockWaveLeftEmitter_ = std::make_unique<EmitterGroup>();

	shockWaveLeftEmitter_->Initialize(camera_.get());

	shockWaveLeftEmitter_->LoadEmitter("ShockWaveLeft");

	//衝撃波エミッター(右)

	shockWaveRightEmitter_ = std::make_unique<EmitterGroup>();

	shockWaveRightEmitter_->Initialize(camera_.get());

	shockWaveRightEmitter_->LoadEmitter("ShockWaveRight");

	Fade::GetInstance()->SetCamera(camera_.get());

	Fade::GetInstance()->SetPlayer(player_);

	Fade::GetInstance()->StartFadeIn();

}

void GameScene3DObject::Update() {

	//追尾カメラの更新
	followCamera_->Update();

	//カメラをデバッグ状態で更新
	camera_->Update();

	if (!isClear_) {

		//プレイヤーの更新
		player_->Update();

		//エネミーの更新
		enemyManager_->Update();

		//弾の更新
		bulletManager_->Update();

		//グラウンドマネージャーの更新
		groundManager_->Update();
	} else {

		player_->TransformUpdate();

		enemyManager_->TransformUpdate();

		bulletManager_->TransformUpdate();

		groundManager_->TransformUpdate();
	}

	//右衝撃波エミッターの更新
	shockWaveRightEmitter_->Update();

	//左衝撃波エミッターの更新
	shockWaveLeftEmitter_->Update();

}

void GameScene3DObject::Draw() {

	//プレイヤーの描画
	player_->Draw();

	//エネミーの描画
	enemyManager_->Draw();

	//弾の描画
	bulletManager_->Draw();

	//グラウンドマネージャーの描画
	groundManager_->Draw();

	//右衝撃波エミッターの描画
	shockWaveRightEmitter_->Draw();

	//左衝撃波エミッターの描画
	shockWaveLeftEmitter_->Draw();

}

void GameScene3DObject::StartClearAnimation() {

	enemyManager_->StartClearUpdate();

	player_->SetIsMoveActive(false);
}

void GameScene3DObject::ClearAnimation(AnimPoint prePoint, AnimPoint currentPoint, float time) {

	//キーフレーム間を補間
	player_->SetPosition(EaseOut(prePoint.playerPos, currentPoint.playerPos, time, currentPoint.mag));

	shockWaveLeftEmitter_->GetWorldTransform().translate_ = player_->GetWorldPos() + Vector3(-1.75f, 0.0f, 0.0f);

	shockWaveRightEmitter_->GetWorldTransform().translate_ = player_->GetWorldPos() + Vector3(1.75f, 0.0f, 0.0f);

}

void GameScene3DObject::ClearParticleEmit() {

	shockWaveLeftEmitter_->GetWorldTransform().translate_ = player_->GetWorldPos() + Vector3(-1.75f, 0.0f, 0.0f);
	shockWaveLeftEmitter_->Emit();

	shockWaveRightEmitter_->GetWorldTransform().translate_ = player_->GetWorldPos() + Vector3(1.75f, 0.0f, 0.0f);
	shockWaveRightEmitter_->Emit();
}

void GameScene3DObject::StartAnimation(AnimPoint prePoint, AnimPoint currentPoint, float time) {

	//キーフレーム間を補間
	player_->SetPosition(EaseOut(prePoint.playerPos, currentPoint.playerPos, time, currentPoint.mag));

	camera_->GetWorldTransform().rotate_ = EaseOut(prePoint.cameraRot, currentPoint.cameraRot, time, currentPoint.mag);

}

void GameScene3DObject::StartAnimEnd() {

	//追従カメラを有効化
	followCamera_->SetIsActive(true);

	//プレイヤーが動けるようにする
	player_->SetIsMoveActive(true);
}