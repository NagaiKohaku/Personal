#include "ColliderManager.h"
#include "AABBCollider.h"
#include "SphereCollider.h"

#include "algorithm"

///=====================================================/// 
/// シングルトンインスタンスの取得
///=====================================================///
ColliderManager* ColliderManager::GetInstance() {
	static ColliderManager instance;
	return &instance;
}

///=====================================================/// 
/// 初期化
///=====================================================///
void ColliderManager::Initialize() {

	//リストのクリア
	colliders_.clear();
}

///=====================================================/// 
/// 更新
///=====================================================///
void ColliderManager::Update() {

	//全コライダーの接触判定
	CheckAllCollision();
}

///=====================================================/// 
/// 描画
///=====================================================///
void ColliderManager::Draw() {
}

///=====================================================/// 
/// コライダーの追加
///=====================================================///
void ColliderManager::AddCollider(Collider* collider) {

	//リストに追加
	colliders_.push_back(collider);
}

///=====================================================/// 
/// コライダーの削除
///=====================================================///
void ColliderManager::RemoveCollider(Collider* collider) {

	//リストから引数のコライダーを探索
	auto it = std::find(colliders_.begin(), colliders_.end(), collider);

	//コライダーがあった場合
	if (it != colliders_.end()) {

		//リストから削除
		colliders_.erase(it);
	}
}

///=====================================================/// 
/// 全コライダーの接触判定
///=====================================================///
void ColliderManager::CheckAllCollision() {

	for (auto it1 = colliders_.begin(); it1 != colliders_.end(); ++it1) {

		//非アクティブだった場合スキップ
		if (!(*it1)->GetIsActive()) {
			continue;
		}
		for (auto it2 = std::next(it1); it2 != colliders_.end(); ++it2) {

			//非アクティブだった場合スキップ
			if (!(*it2)->GetIsActive()) {
				continue;
			}

			//接触していた場合
			if (CheckPair(*it1, *it2)) {

				//接触相手のタグを登録
				(*it1)->AddHitTag((*it2)->GetTag());
				(*it2)->AddHitTag((*it1)->GetTag());
			}
		}
	}
}

///=====================================================/// 
/// 2つのコライダーの接触判定
///=====================================================///
bool ColliderManager::CheckPair(Collider* first, Collider* second) {

	//AABB同士の場合
	if (dynamic_cast<AABBCollider*>(first) && dynamic_cast<AABBCollider*>(second)) {
		return IsCollisionAABBWithAABB(first, second);
	}

	//Sphere同士の場合
	if (dynamic_cast<SphereCollider*>(first) && dynamic_cast<SphereCollider*>(second)) {
		return IsCollisionSphereWithSphere(first, second);
	}

	//1つ目がAABB、2つ目がSphereの場合
	if ((dynamic_cast<AABBCollider*>(first) && dynamic_cast<SphereCollider*>(second))) {
		return IsCollisionAABBWithSphere(first, second);
	}

	//1つ目がSphere、2つ目がAABBの場合
	if ((dynamic_cast<SphereCollider*>(first) && dynamic_cast<AABBCollider*>(second))) {
		return IsCollisionAABBWithSphere(second, first);
	}

	return false;
}

///=====================================================/// 
/// AABBとAABBの接触判定
///=====================================================///
bool ColliderManager::IsCollisionAABBWithAABB(Collider* first, Collider* second) {

	//コライダーをAABBコライダーに変換
	AABBCollider* firstAABBCol = dynamic_cast<AABBCollider*>(first);
	AABBCollider* secondAABBCol = dynamic_cast<AABBCollider*>(second);

	//AABB情報を取得
	AABB firstAABB = firstAABBCol->GetAABB();
	AABB secondAABB = secondAABBCol->GetAABB();

	//中心座標取得
	Vector3 firstWorldTranslate = firstAABBCol->GetWorldTransform().GetWorldTranslate();
	Vector3 secondWorldTranslate = secondAABBCol->GetWorldTransform().GetWorldTranslate();

	//コライダーの現在座標からAABBを計算
	firstAABB = {
		firstAABB.min + firstWorldTranslate,
		firstAABB.max + firstWorldTranslate
	};
	secondAABB = {
		secondAABB.min + secondWorldTranslate,
		secondAABB.max + secondWorldTranslate
	};

	//接触している場合
	if (firstAABB.min.x <= secondAABB.max.x && firstAABB.max.x >= secondAABB.min.x &&
		firstAABB.min.y <= secondAABB.max.y && firstAABB.max.y >= secondAABB.min.y &&
		firstAABB.min.z <= secondAABB.max.z && firstAABB.max.z >= secondAABB.min.z) {

		return true;
	}

	return false;
}

///=====================================================/// 
/// SphereとSphereの接触判定
///=====================================================///
bool ColliderManager::IsCollisionSphereWithSphere(Collider* first, Collider* second) {

	//コライダーをSphereコライダーに変換
	SphereCollider* firstSphereCol = dynamic_cast<SphereCollider*>(first);
	SphereCollider* secondSphereCol = dynamic_cast<SphereCollider*>(second);

	//Sphere情報を取得
	Sphere firstSphere = firstSphereCol->GetSphere();
	Sphere secondSphere = secondSphereCol->GetSphere();

	//中心座標取得
	Vector3 firstWorldTranslate = firstSphereCol->GetWorldTransform().GetWorldTranslate();
	Vector3 secondWorldTranslate = secondSphereCol->GetWorldTransform().GetWorldTranslate();

	//Sphereの中心座標を計算
	firstSphere.center += firstWorldTranslate;
	secondSphere.center += secondWorldTranslate;

	//Sphereの距離
	float distance = Length(firstSphere.center - secondSphere.center);

	//接触している場合
	if (distance <= firstSphere.radius + secondSphere.radius) {
		return true;
	}

	return false;
}

///=====================================================/// 
/// AABBとSphereの接触判定
///=====================================================///
bool ColliderManager::IsCollisionAABBWithSphere(Collider* aabb, Collider* sphere) {

	//AABBコライダーに変換
	AABBCollider* aabbCol = dynamic_cast<AABBCollider*>(aabb);

	//Sphereコライダーに変換
	SphereCollider* sphereCol = dynamic_cast<SphereCollider*>(sphere);

	//AABB情報を取得
	AABB firstAABB = aabbCol->GetAABB();

	//Sphere情報を取得
	Sphere secondSphere = sphereCol->GetSphere();

	//中心座標取得
	Vector3 firstWorldTranslate = aabbCol->GetWorldTransform().GetWorldTranslate();
	Vector3 secondWorldTranslate = sphereCol->GetWorldTransform().GetWorldTranslate();

	//コライダーの現在座標からAABBを計算
	firstAABB = {
		firstAABB.min + firstWorldTranslate,
		firstAABB.max + firstWorldTranslate
	};

	//Sphereの中心座標を計算
	secondSphere.center += secondWorldTranslate;

	//最近接点を計算
	Vector3 closestPoint = {
		std::clamp(secondSphere.center.x, firstAABB.min.x, firstAABB.max.x),
		std::clamp(secondSphere.center.y, firstAABB.min.y, firstAABB.max.y),
		std::clamp(secondSphere.center.z, firstAABB.min.z, firstAABB.max.z)
	};

	//距離を計算
	float distance = Length(closestPoint - secondSphere.center);

	//接触している場合
	if (distance <= secondSphere.radius) {

		return true;
	}

	return false;
}