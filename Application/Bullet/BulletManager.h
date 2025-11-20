#pragma once

#include "Bullet/BulletBase.h"

#include "list"

///=====================================================/// 
/// バレットマネージャークラス
///=====================================================///
class BulletManager {

	///-------------------------------------------/// 
	/// 列挙型
	///-------------------------------------------///
public:

	enum BULLETTYPE {
		TANK,
		JET,
		ENEMY
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
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 座標のみ更新
	/// </summary>
	void TransformUpdate();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 弾を追加
	/// </summary>
	/// <param name="pos">初期座標</param>
	/// <param name="direction">発射方向</param>
	void AddBullet(Vector3 pos, Vector3 direction, BULLETTYPE type);

	///-------------------------------------------/// 
	/// クラス内処理関数
	///-------------------------------------------///
private:

	/// <summary>
	/// 弾インスタンスを生成
	/// </summary>
	/// <param name="type">タイプ</param>
	/// <returns>インスタンス</returns>
	std::unique_ptr<BulletBase> CreateBullet(BULLETTYPE type);

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//弾リスト
	std::list<std::unique_ptr<BulletBase>> bullets_;
};