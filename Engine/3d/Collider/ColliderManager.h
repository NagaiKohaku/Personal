#pragma once

#include "Collider.h"

#include "list"
#include "memory"

///=====================================================/// 
/// コライダーマネージャークラス
///=====================================================///
class ColliderManager {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// ColliderManagerのシングルトンインスタンスを取得します。
	/// </summary>
	static ColliderManager* GetInstance();

	/// <summary>
	/// ColliderManager を初期化します。
	/// </summary>
	void Initialize();

	/// <summary>
	/// ColliderManager を更新します。
	/// </summary>
	void Update();

	/// <summary>
	/// コライダーを登録します。
	/// </summary>
	/// <param name="collider"> 登録するコライダー </param>
	void AddCollider(Collider* collider);

	/// <summary>
	/// 指定されたコライダーを登録リストから削除します。
	/// </summary>
	/// <param name="collider"> 削除するコライダー </param>
	void RemoveCollider(Collider* collider);

	/// <summary>
	/// 管理しているすべてのコライダーをクリアします。
	/// </summary>
	void ClearColliders() { colliders_.clear(); }

	///-------------------------------------------/// 
	/// クラス内処理関数
	///-------------------------------------------///
private:

	/// <summary>
	/// 登録されている全てのコライダー同士の衝突判定を行います。
	/// </summary>
	void CheckAllCollision();

	/// <summary>
	/// 2つのコライダー同士の衝突判定を行います。
	/// </summary>
	/// <param name="first">1つ目のコライダー</param>
	/// <param name="second">2つ目のコライダー</param>
	/// <returns>衝突している場合は true、していない場合は false</returns>
	bool CheckPair(Collider* first, Collider* second);

	/// <summary>
	/// 2つのAABBCollider同士の衝突判定を行います。
	/// </summary>
	/// <param name="first">1つ目のコライダー</param>
	/// <param name="second">2つ目のコライダー</param>
	/// <returns>衝突している場合は true、していない場合は false</returns>
	bool IsCollisionAABBWithAABB(Collider* first, Collider* second);

	/// <summary>
	/// 2つのSphereCollider同士の衝突判定を行います。
	/// </summary>
	/// <param name="first">1つ目のコライダー</param>
	/// <param name="second">2つ目のコライダー</param>
	/// <returns>衝突している場合は true、していない場合は false</returns>
	bool IsCollisionSphereWithSphere(Collider* first, Collider* second);

	/// <summary>
	/// AABBCollider と SphereCollider の衝突判定を行います。
	/// </summary>
	/// <param name="aabb">AABB コライダー</param>
	/// <param name="sphere">Sphere コライダー</param>
	/// <returns>衝突している場合は true、していない場合は false</returns>
	bool IsCollisionAABBWithSphere(Collider* first, Collider* second);

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//コライダーリスト
	std::list<Collider*> colliders_;
};