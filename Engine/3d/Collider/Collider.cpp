#include "Collider.h"

#include "Math/MakeMatrixMath.h"

///=====================================================/// 
/// 初期化
///=====================================================///
void Collider::Initialize(WorldTransform* parent) {

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	//親オブジェクトのワールドトランスフォームを取得
	parentTransform_ = parent;

	//描画フラグの設定
	isDraw_ = false;

	//アクティブフラグの設定
	isActive_ = true;

	//通常カラーの設定
	defaultColor_ = { 1.0f,1.0f,1.0f,1.0f };

	//接触時カラーの設定
	hitColor_ = { 0.0f,1.0f,0.0f,1.0f };

	//デバッグオブジェクトの生成
	debugObject_ = std::make_unique<DebugObject3D>();

	//デバッグオブジェクトの初期化
	debugObject_->Initialize();

}

///=====================================================/// 
/// 更新
///=====================================================///
void Collider::Update() {

	//ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();

	//親オブジェクトのワールド行列からワールド座標を取得
	Matrix4x4 parentTransform = parentTransform_->GetWorldTranslateMatrix();

	Matrix4x4 parentRotate = MakeIdentity4x4();

	Matrix4x4 parentScale = MakeIdentity4x4();

	Matrix4x4 parentWorldMatrix = (parentScale * parentRotate) * parentTransform;

	//親オブジェクトの位置のみ適用して座標を設定
	worldTransform_.SetWorldMatrix(worldTransform_.GetWorldMatrix() * parentWorldMatrix);

	//デバッグオブジェクトの更新
	debugObject_->Update();

	//タグが更新されていなければ
	if (hitTag_ != NONE) {
		isCollision_ = true;
	} else {
		isCollision_ = false;
	}

	//1フレーム前と接触相手のタグに変更があれば
	if (hitTag_ != hitTagBefore_) {
		isTrigger_ = true;
	} else {
		isTrigger_ = false;
	}

	//1フレーム前のタグの更新
	hitTagBefore_ = hitTag_;

	//接触していれば
	if (isCollision_) {
		debugObject_->GetModel()->SetColor(hitColor_);
	} else {
		debugObject_->GetModel()->SetColor(defaultColor_);
	}

	//接触タグのリセット
	hitTag_ = NONE;
}

///=====================================================/// 
/// 描画
///=====================================================///
void Collider::Draw() {

	//描画しない場合は早期リターン
	if (!isDraw_) {
		return;
	}

	//デバッグオブジェクトの描画
	debugObject_->Draw();
}
