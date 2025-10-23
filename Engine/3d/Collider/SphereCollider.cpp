#include "SphereCollider.h"

#include "3d/Model/ModelManager.h"
#include "ColliderManager.h"

#include "numbers"

///=====================================================/// 
/// SphereCollider を初期化
///=====================================================///
void SphereCollider::Initialize(WorldTransform* parent) {

	//コライダーの初期化
	Collider::Initialize(parent);

	//球の初期化
	sphere_.center = { 0.0f, 0.0f, 0.0f };
	sphere_.radius = 1.0f; // 半径1の球

	//球の中心座標に設定
	worldTransform_.translate_ = sphere_.center;

	//球の大きさで設定
	worldTransform_.scale_ = { sphere_.radius, sphere_.radius, sphere_.radius };

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
/// SphereCollider を毎フレーム更新
///=====================================================///
void SphereCollider::Update() {

	//中心座標の更新
	worldTransform_.translate_ = sphere_.center;

	//半径の更新
	worldTransform_.scale_ = { sphere_.radius, sphere_.radius, sphere_.radius };

	//コライダーの更新
	Collider::Update();
}

///=====================================================/// 
/// SphereCollider を描画
///=====================================================///
void SphereCollider::Draw() {

	//コライダーの描画
	Collider::Draw();
}

///=====================================================/// 
/// SphereCollider をコライダーマネージャーから削除
///=====================================================///
void SphereCollider::Remove() {

	//コライダーマネージャーから削除
	ColliderManager::GetInstance()->RemoveCollider(this);
}