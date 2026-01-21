#pragma once

#include "Bullet/BulletBase.h"

#include "list"

/// <summary>
/// 弾丸の生成・管理・更新・描画を統括する仕組みです。
/// </summary>
class BulletManager {

	///-------------------------------------------/// 
	/// 列挙型
	///-------------------------------------------///
public:

	/// <summary>
	/// 弾の種類
	/// </summary>
	enum class BulletType {
		TANK, //戦車状態の弾
		JET,  //戦闘機状態の弾
		ENEMY //敵の弾
	};

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 登録されているすべての弾を更新します。
	/// </summary>
	void Update();

	/// <summary>
	/// すべての弾の座標情報を更新します。
	/// </summary>
	void TransformUpdate();

	/// <summary>
	/// 登録されているすべての弾を描画します。
	/// </summary>
	void Draw();

	/// <summary>
	/// 新しい弾を生成して管理リストに追加します。
	/// </summary>
	void AddBullet(Vector3 pos, Vector3 direction, BulletType type);

	///-------------------------------------------/// 
	/// クラス内処理関数
	///-------------------------------------------///
private:

	/// <summary>
	/// 指定された種類に応じた弾のインスタンスを生成します。
	/// </summary>
	std::unique_ptr<BulletBase> CreateBullet(BulletType type);

	/// <summary>
	/// 削除対象の弾をリストから削除します。
	/// </summary>
	void RemoveBullet();

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//弾リスト
	std::list<std::unique_ptr<BulletBase>> bullets_;
};