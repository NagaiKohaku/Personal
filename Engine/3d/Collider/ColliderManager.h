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
	/// <remarks>
	/// 返り値に静的インスタンスを返します。
	/// </remarks>
	static ColliderManager* GetInstance();

	/// <summary>
	/// ColliderManager を初期化します。
	/// </summary>
	/// <remarks>
	/// - コライダーリストを初期状態に戻し、次のフレームから新規登録を受け付ける準備を行います。  
	/// </remarks>
	void Initialize();

	/// <summary>
	/// ColliderManager を更新します。
	/// </summary>
	/// <remarks>
	/// - 登録されているすべてのコライダー同士の衝突判定を実行します。  
	/// - 各コライダー間での接触情報を更新し、衝突・トリガーの状態を反映します。  
	/// </remarks>
	void Update();

	/// <summary>
	/// コライダーを登録します。
	/// </summary>
	/// <param name="collider"> 登録するコライダー </param>
	/// <remarks>
	/// - 渡されたコライダーを管理リストに追加します。  
	/// </remarks>
	void AddCollider(Collider* collider);

	/// <summary>
	/// 指定されたコライダーを登録リストから削除します。
	/// </summary>
	/// <param name="collider"> 削除するコライダー </param>
	/// <remarks>
	/// - 一致するコライダーが見つかった場合、その要素をリストから削除します。  
	/// </remarks>
	void RemoveCollider(Collider* collider);

	/// <summary>
	/// 管理しているすべてのコライダーをクリアします。
	/// </summary>
	/// <remarks>
	/// - コライダーリストを空にします。  
	/// </remarks>
	void ClearColliders() { colliders_.clear(); }

	///-------------------------------------------/// 
	/// クラス内処理関数
	///-------------------------------------------///
private:

	/// <summary>
	/// 登録されている全てのコライダー同士の衝突判定を行います。
	/// </summary>
	/// <remarks>
	/// - アクティブ状態のコライダーのみを対象とします。
	/// - 各コライダーの接触相手タグを更新します。
	/// </remarks>
	void CheckAllCollision();

	/// <summary>
	/// 2つのコライダー同士の衝突判定を行います。
	/// </summary>
	/// <param name="first">1つ目のコライダー</param>
	/// <param name="second">2つ目のコライダー</param>
	/// <returns>衝突している場合は true、していない場合は false</returns>
	/// <remarks>
	/// - 判定対象は AABB と Sphere の組み合わせをサポートしています。
	/// - 具体的には以下の組み合わせに対応します:
	///   - AABB vs AABB
	///   - Sphere vs Sphere
	///   - AABB vs Sphere / Sphere vs AABB
	/// - その他の組み合わせの場合は false を返します。
	/// </remarks>
	bool CheckPair(Collider* first, Collider* second);

	/// <summary>
	/// 2つのAABBCollider同士の衝突判定を行います。
	/// </summary>
	/// <param name="first">1つ目のコライダー</param>
	/// <param name="second">2つ目のコライダー</param>
	/// <returns>衝突している場合は true、していない場合は false</returns>
	/// <remarks>
	/// - 引数のColliderをAABBColliderにキャストして扱います。
	/// - 各AABBの最小座標・最大座標にワールド座標を加算して現在座標に変換します。
	/// - 3軸(x, y, z)すべてで範囲が重なっている場合に衝突と判定します。
	/// </remarks>
	bool IsCollisionAABBWithAABB(Collider* first, Collider* second);

	/// <summary>
	/// 2つのSphereCollider同士の衝突判定を行います。
	/// </summary>
	/// <param name="first">1つ目のコライダー</param>
	/// <param name="second">2つ目のコライダー</param>
	/// <returns>衝突している場合は true、していない場合は false</returns>
	/// <remarks>
	/// - 引数のColliderをSphereColliderにキャストして扱います。
	/// - 各Sphereの中心座標にワールド座標を加算して現在座標に変換します。
	/// - 2つの中心間距離が半径の和以下であれば衝突と判定します。
	/// </remarks>
	bool IsCollisionSphereWithSphere(Collider* first, Collider* second);

	/// <summary>
	/// AABBCollider と SphereCollider の衝突判定を行います。
	/// </summary>
	/// <param name="aabb">AABB コライダー</param>
	/// <param name="sphere">Sphere コライダー</param>
	/// <returns>衝突している場合は true、していない場合は false</returns>
	/// <remarks>
	/// - 引数の Collider をそれぞれ AABBCollider と SphereCollider にキャストして扱います。
	/// - AABB と Sphere のワールド座標を考慮して現在の座標を計算します。
	/// - Sphere の中心に対して AABB の最近接点を求めます。
	/// - その最近接点と Sphere の中心の距離が半径以下であれば衝突と判定します。
	/// </remarks>
	bool IsCollisionAABBWithSphere(Collider* first, Collider* second);

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//コライダーリスト
	std::list<Collider*> colliders_;
};