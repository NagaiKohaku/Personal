#include "AABBCollider.h"

#include "ColliderManager.h"
#include "3d/Model/ModelManager.h"

#include "cstdlib"

namespace MyEngine {

	///=====================================================/// 
	/// AABBCollider を初期化
	///=====================================================///
	void AABBCollider::Initialize(WorldTransform* parent) {

		//コライダーの初期化
		Collider::Initialize(parent);

		// AABBの初期化
		aabb_.min = { -1.0f, -1.0f, -1.0f };
		aabb_.max = { 1.0f, 1.0f, 1.0f };

		//AABBの中心に座標を設定
		worldTransform_.translate_ = {
			(aabb_.min.x + aabb_.max.x) / 2.0f,
			(aabb_.min.y + aabb_.max.y) / 2.0f,
			(aabb_.min.z + aabb_.max.z) / 2.0f
		};

		//AABBのサイズでスケールを設定
		worldTransform_.scale_ = {
			(std::fabsf(aabb_.min.x) + std::fabsf(aabb_.max.x)) / 2.0f,
			(std::fabsf(aabb_.min.y) + std::fabsf(aabb_.max.y)) / 2.0f,
			(std::fabsf(aabb_.min.z) + std::fabsf(aabb_.max.z)) / 2.0f
		};

		//球体モデルを作成
		ModelManager::GetInstance()->CreateMeshModel("SphereMesh", MeshType::SPHERE, "Resource/Texture/white_128x128.png");

		//デバッグオブジェクトに球体モデルを設定
		debugObject_->SetModel("SphereMesh");

		//親オブジェクトのワールドトランスフォームとペアレント設定
		debugObject_->GetWorldTransform().SetParent(&worldTransform_);

		//コライダーマネージャーに登録
		ColliderManager::GetInstance()->AddCollider(this);
	}

	///=====================================================/// 
	/// AABBCollider を更新
	///=====================================================///
	void AABBCollider::Update() {

		//コライダーの更新
		Collider::Update();
	}

	///=====================================================/// 
	/// AABBCollider を描画
	///=====================================================///
	void AABBCollider::Draw() {

		//コライダーの描画
		Collider::Draw();
	}

	void AABBCollider::SetTransform(Vector3 pos, Vector3 scale) {

		worldTransform_.translate_ = pos;

		worldTransform_.scale_ = scale;

		Vector3 minScale = { -scale.x,-scale.y,-scale.z };

		Vector3 maxScale = { scale.x,scale.y,scale.z };

		aabb_.min = minScale;

		aabb_.max = maxScale;
	}
}