#include "Shadow.h"

#include "3d/Model/Model.h"

#include "Math/Easing.h"

void Shadow::Initialize() {
	// オブジェクトの生成
	shadow_ = std::make_unique<Object3D>();

	// オブジェクトの初期化
	shadow_->Initialize();

	shadow_->SetModel("Sphere");

	shadow_->GetWorldTransform().scale_ = { 1.0f,0.01f,1.0f };

	shadow_->GetModel()->SetColor({ 0.0f,0.0f,0.0f,1.0f });

	shadow_->GetModel()->SetEnvironmentCoefficient(0.0f);

	maxLength_ = 10.0f;

	maxScale_ = 1.5f;

	isDraw_ = true;
}

void Shadow::Update(Vector3 pos) {

	shadow_->GetWorldTransform().translate_ = { pos.x,0.1f,pos.z };

	float length = maxLength_ - pos.y;

	float scale = Lerp(0.0f, maxScale_, length / maxLength_);

	if (scale <= 0.1f) {
		scale = 0.1f;
	}

	shadow_->GetWorldTransform().scale_ = { scale,0.01f,scale };

	// オブジェクトの更新
	shadow_->Update();

	if (pos.y <= 0.0f) {

		isDraw_ = false;
	}
	else {
		isDraw_ = true;
	}
}

void Shadow::Draw() {

	if (isDraw_) {

		// オブジェクトの描画
		shadow_->Draw(LayerType::Object);
	}
}