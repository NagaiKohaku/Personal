#include "ClearEvent.h"

#include <ObjectManager.h>
#include <Shake/Shake.h>
#include <Fade/Fade.h>
#include <Flash/Flash.h>
#include <Base/OffScreen.h>
#include <Base/Input.h>

void ClearEvent::Initialize(GameScene3DObject* object3D, GameScene2DObject* object2D) {

	object2D_ = object2D;

	killCountMax_ = 30;

	isClear_ = false;

	isAnimation_ = false;

	clearAnimPoints_.push_back({ Vector3(0.0f,0.0f,0.0f),Vector3(0.2f,0.0f,0.0f),0.0f,1.0f });
	clearAnimPoints_.push_back({ Vector3(0.0f,0.0f,0.0f),Vector3(0.2f,0.0f,0.0f),0.0f,1.0f });
	clearAnimPoints_.push_back({ Vector3(0.0f,4.0f,0.0f),Vector3(0.2f,0.0f,0.0f),1.5f,2.0f });
	clearAnimPoints_.push_back({ Vector3(0.0f,40.0f,600.0f),Vector3(0.2f,0.0f,0.0f),2.5f,2.0f });

}

void ClearEvent::Update() {

	Animation();

	if (ObjectManager::GetInstance()->GetKillCount() >= killCountMax_) {

		if (Fade::GetInstance()->GetState() != Fade::FADE_OUT && Fade::GetInstance()->GetState() != Fade::FADE_OUT_END) {

			if (!isClear_) {

				if (!isAnimation_) {

					isClear_ = true;

					object2D_->ShowSprite();

					object3D_->StartClearAnimation();

					clearAnimPoints_[0].playerPos = object3D_->GetPlayerPos();
					clearAnimPoints_[1].playerPos = object3D_->GetPlayerPos();

					//シェイクを始める
					Shake::GetInstance()->Start(1.0f, 1.0f);

					//画面をフラッシュさせる
					Flash::GetInstance()->Start(0.5f, Vector4(0.0f, 0.0f, 0.0f, 1.0f));

					//色を反転させる
					OffScreen::GetInstance()->SetColorReverseRatio(0.75f);
				}
			}
		}
	}

	if (isClear_) {

		if (Input::GetInstance()->IsTriggerPushKey(DIK_SPACE)) {

			isAnimation_ = true;

			object2D_->HideSprite();

			//画面をフラッシュさせる
			Flash::GetInstance()->Start(0.5f, Vector4(1.0f, 1.0f, 1.0f, 1.0f));

			//色を反転させる
			OffScreen::GetInstance()->SetColorReverseRatio(0.0f);
		}
	}

}

void ClearEvent::Animation() {

	//スタート時以外はスキップ
	if (!isAnimation_) {
		return;
	}

	//タイマーを進ませる
	timer_ += 1.0f / 60.0f;

	//アニメーションに進捗
	float ratio;

	//最初のアニメーションであれば
	if (animNum_ == 1) {

		//進捗を固定
		ratio = 1.0f;
	} else {

		//進捗を計算
		ratio = (timer_ - clearAnimPoints_[animNum_ - 1].time) / (clearAnimPoints_[animNum_].time - clearAnimPoints_[animNum_ - 1].time);
	}

	//進捗が1以上になったらそろえる
	if (ratio >= 1.0f) {

		ratio = 1.0f;
	}

	//3Dオブジェクトのアニメーション
	object3D_->ClearAnimation(clearAnimPoints_[animNum_ - 1], clearAnimPoints_[animNum_], ratio);

	//進捗が最大値になっていたら
	if (ratio == 1.0f) {

		if (animNum_ == static_cast<int>(clearAnimPoints_.size()) - 1) {

			object3D_->ClearParticleEmit();

			//フェードアウトを始める
			Fade::GetInstance()->StartFadeOut();
		}

		//アニメーション番号を進ませる
		animNum_++;
	}

	//アニメーション番号が最後であれば
	if (animNum_ == static_cast<int>(clearAnimPoints_.size())) {

		//スタート時の演出を終わる
		isClear_ = false;
	}

}