#include "ClearEvent.h"

#include <UI/UIManager.h>
#include <Effect/Flash/Flash.h>
#include <Effect/Shake/Shake.h>
#include <Effect/Fade/Fade.h>
#include <Base/OffScreen.h>
#include <Base/Input.h>
#include <Math/Utility/MakeMatrixMath.h>

using namespace MyEngine;

void ClearEvent::Start(MyEngine::EngineContext context, Player* player, FollowCamera* followCamera) {

	context_ = context;
	player_ = player;

	motionNum_ = 1;

	canMove_ = false;

	motionPoint_.push_back({ player_->GetWorldPos(),context_.camera->GetWorldTransform().rotate_,0.0f,EaseType::LINEAR,1.0f });
	motionPoint_.push_back({ Vector3(0.0f,4.0f,0.0f),Vector3(0.0f,0.0f,0.0f),1.5f,EaseType::EASE_OUT,2.0f });
	motionPoint_.push_back({ Vector3(0.0f,40.0f,600.0f),Vector3(0.0f,0.0f,0.0f),2.5f,EaseType::EASE_OUT,2.0f });

	player_->SetIsMoveActive(false);

	/// === エミッターの生成 === ///

	//衝撃波エミッター(左)
	shockWaveLeftEmitter_ = std::make_unique<EmitterGroup>();

	shockWaveLeftEmitter_->Initialize(context_.camera);

	shockWaveLeftEmitter_->LoadEmitter("ShockWaveLeft");

	//衝撃波エミッター(右)

	shockWaveRightEmitter_ = std::make_unique<EmitterGroup>();

	shockWaveRightEmitter_->Initialize(context_.camera);

	shockWaveRightEmitter_->LoadEmitter("ShockWaveRight");

	//スプライトを映す
	UIManager::GetInstance()->GetUIGroup("Clear")->isActive = true;

	//テキスト系のUIを隠す
	UIManager::GetInstance()->GetUIGroup("Reticle")->isActive = false;

	UIManager::GetInstance()->GetUIGroup("Help")->isActive = false;

	spaceKeyPos_ = UIManager::GetInstance()->Get2DObject("Clear", "Space")->GetTranslate();

	spaceKeySize_ = UIManager::GetInstance()->Get2DObject("Clear", "Space")->GetSize();

	//シェイクを始める
	Shake::GetInstance()->Start(1.0f, 1.0f);

	//画面をフラッシュさせる
	Flash::GetInstance()->Start(0.5f, Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	//色を反転させる
	context_.offScreen->SetColorReverseRatio(0.75f);

	arrowLength_ = 20.0f;

	arrowTimer_ = 0.0f;

	timerDirection_ = 1.0f;
}

void ClearEvent::Exit() {
}

void ClearEvent::Update() {

	arrowTimer_ += (1.0f / 60.0f) * timerDirection_;

	sceneChangeTimer_ += 1.0f / 60.0f;

	if (arrowTimer_ >= 1.0f) {

		arrowTimer_ = 1.0f;

		timerDirection_ *= -1.0f;
	}

	if (arrowTimer_ <= 0.0f) {

		arrowTimer_ = 0.0f;

		timerDirection_ *= -1.0f;
	}

	//右衝撃波エミッターの更新
	shockWaveRightEmitter_->Update();

	//左衝撃波エミッターの更新
	shockWaveLeftEmitter_->Update();

	shockWaveRightEmitter_->GetWorldTransform().translate_ = player_->GetWorldPos() + Vector3(1.75f, 0.0f, 0.0f);

	shockWaveLeftEmitter_->GetWorldTransform().translate_ = player_->GetWorldPos() + Vector3(-1.75f, 0.0f, 0.0f);

	float lerpNum = EaseOut(0.0f, arrowLength_, arrowTimer_ / 1.0f);

	//3Dオブジェクトの座標をスクリーン座標に変換する
	Vector3 playerScreenPos = Vector3ToScreenSpace(context_.camera, player_->GetWorldPos());

	UIManager::GetInstance()->Get2DObject("Clear", "LeftArrow")->SetTranslate({ spaceKeyPos_.x - spaceKeySize_.x / 2.0f - 64.0f - lerpNum,spaceKeyPos_.y });

	UIManager::GetInstance()->Get2DObject("Clear", "RightArrow")->SetTranslate({ spaceKeyPos_.x + spaceKeySize_.x / 2.0f + 64.0f + lerpNum,spaceKeyPos_.y });

	if (context_.input->IsTriggerPushKey(DIK_SPACE)) {

		if (sceneChangeTimer_ >= sceneChangeMaxTime_) {

			canMove_ = true;

			//クリアUIを隠す
			UIManager::GetInstance()->GetUIGroup("Clear")->isActive = false;

			//画面をフラッシュさせる
			Flash::GetInstance()->Start(0.5f, Vector4(1.0f, 1.0f, 1.0f, 1.0f));

			//色を反転させる
			context_.offScreen->SetColorReverseRatio(0.0f);
		}
	}

	if (canMove_) {

		UpdateEventMotion();
	}

	if (motionNum_ == static_cast<int>(motionPoint_.size()) - 1) {
		if (!isFade_) {

			Fade::GetInstance()->StartFadeOut();

			shockWaveRightEmitter_->Emit();
			shockWaveLeftEmitter_->Emit();

			isFade_ = true;
		}
	}
}

void ClearEvent::Draw() {

	//右衝撃波エミッターの描画
	shockWaveRightEmitter_->Draw();

	//左衝撃波エミッターの描画
	shockWaveLeftEmitter_->Draw();
}

GameSceneEventBase::EventType ClearEvent::RequestNextEvent() const {

	return EventType::NONE;
}