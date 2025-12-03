#include "Shake.h"

#include "Math/Random.h"
#include "Math/Easing.h"
#include "Math/MakeMatrixMath.h"
#include "Math/Matrix4x4.h"

///=====================================================/// 
/// Shakeのシングルトンインスタンスを取得
///=====================================================///
Shake* Shake::GetInstance() {
	static Shake instance;
	return &instance;
}

///=====================================================/// 
/// シェイク処理用の初期化
///=====================================================///
void Shake::Initialize() {

	transform_.Initialize();

	timer_ = 0.0f;
}

///=====================================================/// 
/// カメラに対してシェイク効果を適用する
///=====================================================///
void Shake::Update() {

	//カメラがなければスキップ
	if (camera_ == nullptr) {

		return;
	}

	//タイマーを進ませる
	timer_ += 1.0f / 60.0f;

	//進捗の計算
	float t = timer_ / shakeTime_;

	//1以上になったらそろえる
	if (t >= 1.0f) {

		t = 1.0f;
	}

	//震える長さを0に近づける
	shakeLength_ = Lerp(shakeLength_, 0.0f, t);

	//カメラのトランスフォームをコピー
	transform_ = camera_->GetWorldTransform();

	//シェイク分を加算
	transform_.translate_ += Vector3(RandomFloat(-shakeLength_, shakeLength_), RandomFloat(-shakeLength_, shakeLength_), 0.0f);

	//トランスフォームを更新
	transform_.UpdateMatrix();

	//ビュー行列の計算
	Matrix4x4 viewMatrix = Inverse4x4(transform_.GetWorldMatrix());

	//ビュープロジェクション行列の計算
	Matrix4x4 viewProjectionMatrix = viewMatrix * camera_->GetProjectionMatrix();

	//カメラに設定
	camera_->SetViewMatrix(viewMatrix);

	camera_->SetViewProjectionMatrix(viewProjectionMatrix);
}

///=====================================================/// 
/// シェイクを開始し初期パラメータを設定する
///=====================================================///
void Shake::Start(float num, float time) {

	timer_ = 0.0f;

	shakeTime_ = time;

	shakeLength_ = num;
}