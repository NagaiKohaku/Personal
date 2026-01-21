#include "Shadow.h"

#include "3d/Model/Model.h"

#include "Math/Easing.h"

///=====================================================/// 
/// 影用オブジェクトを生成し初期設定を行う
///=====================================================///
void Shadow::Initialize() {

	/// === オブジェクトの生成 === ///

	shadow_ = std::make_unique<Object3D>();

	shadow_->Initialize();

	shadow_->SetModel("Sphere");

	shadow_->GetWorldTransform().scale_ = { 1.0f,0.01f,1.0f };

	shadow_->GetModel()->SetColor({ 0.0f,0.0f,0.0f,1.0f });

	//反射係数の設定
	shadow_->GetModel()->SetEnvironmentCoefficient(0.0f);

	/// === 他変数の設定 === ///

	maxLength_ = 10.0f;

	maxScale_ = 1.5f;

	isDraw_ = true;
}

///=====================================================/// 
/// 指定座標に応じて影の位置とサイズを更新
///=====================================================///
void Shadow::Update(Vector3 pos) {

	//引数の座標を設定
	shadow_->GetWorldTransform().translate_ = { pos.x,0.1f,pos.z };

	//進捗の計算
	float length = maxLength_ - pos.y;

	//距離からサイズを計算
	float scale = Lerp(0.0f, maxScale_, length / maxLength_);

	//完全に消えないようにする
	if (scale <= 0.1f) {
		scale = 0.1f;
	}

	//サイズを設定
	shadow_->GetWorldTransform().scale_ = { scale,0.01f,scale };

	// オブジェクトの更新
	shadow_->Update();

	//地面に潜ったら描画しないようにする
	if (pos.y <= 0.0f) {

		isDraw_ = false;
	} else {

		isDraw_ = true;
	}
}

///=====================================================/// 
/// 影オブジェクトを描画
///=====================================================///
void Shadow::Draw() {

	if (isDraw_) {

		// オブジェクトの描画
		shadow_->Draw(LayerType::OBJECT);
	}
}