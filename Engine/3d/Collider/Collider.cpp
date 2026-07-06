#include "Collider.h"

#include <Math/Utility/MakeMatrixMath.h>
#include <3d/Collider/ColliderManager.h>

#include "algorithm"

namespace MyEngine {

	///=====================================================/// 
	/// Collider を初期化
	///=====================================================///
	void Collider::Initialize(WorldTransform* parent) {

		//ワールドトランスフォームの初期化
		worldTransform_.Initialize();

		//親オブジェクトのワールドトランスフォームを取得
		parentTransform_ = parent;

		//識別タグの初期化
		tag_ = Tag::NONE;

		//衝突相手の識別タグの初期化
		hitTag_.clear();

		//1フレーム前の接触相手の識別タグの初期化
		hitTagBefore_.clear();

		//描画フラグの設定
		isDraw_ = true;

		//アクティブフラグの設定
		isActive_ = true;

		//接触フラグの設定
		isCollision_ = false;

		//トリガーフラグの設定
		isTrigger_ = false;

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
	/// Collider の状態を毎フレーム更新
	///=====================================================///
	void Collider::Update() {

		/// === トランスフォームの更新 === ///

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

		/// === 接触判定の更新 === ///

		std::sort(hitTag_.begin(), hitTag_.end());

		//タグが更新されていなければ
		if (hitTag_.size() > 0) {
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
		hitTag_.clear();
	}

	///=====================================================/// 
	/// Collider の描画
	///=====================================================///
	void Collider::Draw() {

		//描画しない場合は早期リターン
		if (!isDraw_) {
			return;
		}

		//デバッグオブジェクトの描画
		debugObject_->Draw();
	}

	void Collider::Remove() {

		//コライダーマネージャーから削除
		ColliderManager::GetInstance()->RemoveCollider(this);
	}
}