#pragma once

#include "3d/Object/Object3D.h"
#include "3d/Object/DebugObject3D.h"
#include "3d/Object/DebugLine.h"

#include "Math/WorldTransform.h"
#include "Math/AABB.h"
#include "Math/Sphere.h"

#include "vector"
#include "memory"

/// <summary>
/// Collider クラスは、ゲームオブジェクトの衝突領域を管理するための基底クラスです。
/// </summary>
/// <remarks>
/// - 他のオブジェクトとの衝突判定を行うための基本的な機能を提供します。  
/// - 各種コライダーは本クラスを継承して実装されます。  
/// - ワールドトランスフォームの更新、識別タグの管理、衝突状態の記録を行います。  
/// </remarks>
class Collider {

	///-------------------------------------------/// 
	/// 列挙型
	///-------------------------------------------///
public:

	//識別タグ
	enum Tag {
		PLAYER,
		ENEMY,
		PLAYERBULLET,
		ENEMYBULLET,
		WALL,
		ITEM,
		NONE
	};


	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// Collider を初期化します。
	/// </summary>
	/// <param name="parent">このコライダーの親となるワールドトランスフォーム</param>
	/// <remarks>
	/// - 自身の worldTransform_ を初期化します。
	/// - 親オブジェクトのワールドトランスフォームを設定します。
	/// - 識別タグを NONE に初期化します。
	/// - 衝突相手のタグリスト hitTag_ と前フレームの hitTagBefore_ をクリアします。
	/// - 描画フラグ、アクティブフラグ、接触フラグ、トリガーフラグを設定します。
	/// - デフォルトカラーと接触時カラーを設定します。
	/// - DebugObject3D のインスタンスを生成し初期化します。
	/// </remarks>
	virtual void Initialize(WorldTransform* parent);

	/// <summary>
	/// Collider の状態を毎フレーム更新します。
	/// </summary>
	/// <remarks>
	/// - ワールドトランスフォームを更新し、親オブジェクトの位置情報を反映します。  
	/// - オブジェクトを更新します。  
	/// - 接触判定情報を基に衝突状態およびトリガー状態を更新します。  
	/// - 最後に現在の接触タグリストをクリアします。  
	/// </remarks>
	virtual void Update();

	/// <summary>
	/// Collider の描画を行います。
	/// </summary>
	virtual void Draw();

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
protected:

	//識別タグ
	Tag tag_;

	//衝突相手の識別タグ
	std::vector<Tag> hitTag_;

	//1フレーム前の衝突相手の識別タグ
	std::vector<Tag> hitTagBefore_;

	//ワールドトランスフォーム
	WorldTransform worldTransform_;

	//親オブジェクトのワールドトランスフォーム
	WorldTransform* parentTransform_;

	//デバッグオブジェクト
	std::unique_ptr<DebugObject3D> debugObject_;

	//通常カラー
	Vector4 defaultColor_;

	//衝突時カラー
	Vector4 hitColor_;

	//描画フラグ
	bool isDraw_;

	//アクティブフラグ
	bool isActive_;

	//接触フラグ
	bool isCollision_;

	//トリガーフラグ
	bool isTrigger_;

	///-------------------------------------------/// 
	/// ゲッター・セッター
	///-------------------------------------------///
public:

	/// <summary>
	/// ワールドトランスフォームを取得
	/// </summary>
	/// <returns>ワールドトランスフォーム</returns>
	WorldTransform& GetWorldTransform() { return worldTransform_; }

	/// <summary>
	/// 識別タグを取得
	/// </summary>
	/// <returns>タグ</returns>
	Tag GetTag() const { return tag_; }

	/// <summary>
	/// 衝突相手の識別タグを取得
	/// </summary>
	/// <returns>タグ</returns>
	std::vector<Tag> GetHitTag() const { return hitTag_; }

	/// <summary>
	/// 指定されたタグとの当たり判定対象かを判定
	/// </summary>
	/// <param name="tag">判定したいタグ</param>
	bool CheckHitTag(Tag tag) const { return std::find(hitTag_.begin(), hitTag_.end(), tag) != hitTag_.end(); }

	/// <summary>
	/// アクティブフラグを取得
	/// </summary>
	/// <returns>フラグ</returns>
	bool GetIsActive() const { return isActive_; }

	/// <summary>
	/// 接触フラグを取得
	/// </summary>
	/// <returns>フラグ</returns>
	bool GetIsCollision() const { return isCollision_; }

	/// <summary>
	/// トリガーフラグを取得
	/// </summary>
	/// <returns>フラグ</returns>
	bool GetIsTrigger() const { return isTrigger_; }

	/// <summary>
	/// 識別タグの設定
	/// </summary>
	/// <param name="tag">タグ</param>
	void SetTag(Tag tag) { tag_ = tag; }

	/// <summary>
	/// 接触相手の識別タグの設定
	/// </summary>
	/// <param name="hitTag">タグ</param>
	void AddHitTag(Tag hitTag) { hitTag_.push_back(hitTag); }

	/// <summary>
	/// 描画フラグの設定
	/// </summary>
	/// <param name="isDraw">フラグ</param>
	void SetIsDraw(bool isDraw) { isDraw_ = isDraw; }

	/// <summary>
	/// アクティブフラグの設定
	/// </summary>
	/// <param name="isActive">フラグ</param>
	void SetIsActive(bool isActive) { isActive_ = isActive; }

};