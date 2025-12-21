#include "GameOverEvent.h"

#include <Shake/Shake.h>
#include <Fade/Fade.h>
#include <Base/OffScreen.h>
#include <Base/Input.h>

void GameOverEvent::Initialize(GameScene3DObject* object3D, GameScene2DObject* object2D) {

	object3D_ = object3D;

	object2D_ = object2D;

	isGameOver_ = false;
}

void GameOverEvent::Update() {

	if (!isGameOver_) {
		if (object3D_->GetPlayerIsDead()) {

			//ゲームオーバー演出を始める
			isGameOver_ = true;

			object2D_->ShowSprite();

			//シェイクを始める
			Shake::GetInstance()->Start(1.0f, 0.5f);

			//色を反転させる
			OffScreen::GetInstance()->SetColorReverseRatio(1.0f);
		}
	} else {

		//ゲームオーバー時スペースキーが押されたら
		if (Input::GetInstance()->IsTriggerPushKey(DIK_SPACE)) {

			//フェードアウトを始める
			Fade::GetInstance()->StartFadeOut();
		}
	}

}