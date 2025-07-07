#pragma once

#include "Collider.h"

#include "list"
#include "memory"

///=====================================================/// 
/// コライダーマネージャー
///=====================================================///
class ColliderManager {

public:

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static ColliderManager* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// コライダーの追加
	/// </summary>
	/// <param name="collider">コライダー</param>
	void AddCollider(Collider* collider);

	/// <summary>
	/// コライダーの削除
	/// </summary>
	/// <param name="collider">コライダー</param>
	void RemoveCollider(Collider* collider);

private:

	/// <summary>
	/// 全コライダーの接触判定
	/// </summary>
	void CheckAllCollision();

	/// <summary>
	/// 2つのコライダーの接触判定
	/// </summary>
	/// <param name="first">1つ目のコライダー</param>
	/// <param name="second">2つ目のコライダー</param>
	/// <returns>判定結果</returns>
	bool CheckPair(Collider* first, Collider* second);

	/// <summary>
	/// AABBとAABBの接触判定
	/// </summary>
	/// <param name="first">1つ目のコライダー</param>
	/// <param name="second">2つ目のコライダー</param>
	/// <returns>判定結果</returns>
	bool IsCollisionAABBWithAABB(Collider* first, Collider* second);

	/// <summary>
	/// SphereとSphereの接触判定
	/// </summary>
	/// <param name="first">1つ目のコライダー</param>
	/// <param name="second">2つ目のコライダー</param>
	/// <returns>判定結果</returns>
	bool IsCollisionSphereWithSphere(Collider* first, Collider* second);

	/// <summary>
	/// AABBとSphereの接触判定
	/// </summary>
	/// <param name="first">AABBのコライダー</param>
	/// <param name="second">Sphereのコライダー</param>
	/// <returns>判定結果</returns>
	bool IsCollisionAABBWithSphere(Collider* first, Collider* second);

private:

	//コライダーリスト
	std::list<Collider*> colliders_;
};