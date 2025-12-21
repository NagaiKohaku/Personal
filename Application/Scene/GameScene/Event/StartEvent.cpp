#include "StartEvent.h"

#include <Math/Easing.h>
#include <numbers>

void StartEvent::Initialize(GameScene3DObject* object3D, GameScene2DObject* object2D) {

	object3D_ = object3D;

	object2D_ = object2D;

	//キーフレームの設定
	startAnimPoints_.push_back({ Vector3(0.0f,20.0f,-600.0f),Vector3(0.0f,-std::numbers::pi_v<float>,0.0f),0.0f,1.0f });
	startAnimPoints_.push_back({ Vector3(0.0f,20.0f,-600.0f),Vector3(0.0f,-std::numbers::pi_v<float>,0.0f),0.0f,1.0f });
	startAnimPoints_.push_back({ Vector3(0.0f,20.0f,-600.0f),Vector3(0.0f,-std::numbers::pi_v<float>,0.0f),1.0f,1.0f });
	startAnimPoints_.push_back({ Vector3(0.0f,0.75f,0.0f),Vector3(0.2f,-std::numbers::pi_v<float>,0.0f),2.0f,3.0f });
	startAnimPoints_.push_back({ Vector3(0.0f,1.0f,0.0f),Vector3(0.2f,0.0f,0.0f),4.0f,4.0f });

	animNum_ = 1;

	isStart_ = true;
}

void StartEvent::Update() {

	Animation();
}

void StartEvent::Animation() {

	//スタート時以外はスキップ
	if (!isStart_) {
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
		ratio = (timer_ - startAnimPoints_[animNum_ - 1].time) / (startAnimPoints_[animNum_].time - startAnimPoints_[animNum_ - 1].time);
	}

	//進捗が1以上になったらそろえる
	if (ratio >= 1.0f) {

		ratio = 1.0f;
	}

	//3Dオブジェクトのアニメーション
	object3D_->StartAnimation(startAnimPoints_[animNum_ - 1], startAnimPoints_[animNum_], ratio);

	//進捗が最大値になっていたら
	if (ratio == 1.0f) {

		//アニメーション番号を進ませる
		animNum_++;
	}

	//アニメーション番号が最後であれば
	if (animNum_ == static_cast<int>(startAnimPoints_.size())) {

		//スタート時の演出を終わる
		isStart_ = false;
	}

}