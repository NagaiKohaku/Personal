#include "BulletManager.h"

#include <Object/Bullet/Player/JetBullet.h>
#include <Object/Bullet/Player/TankBullet.h>
#include <Object/Bullet/Enemy/EnemyBullet.h>

using namespace MyEngine;

///=====================================================/// 
/// 初期化
///=====================================================///
void BulletManager::Initialize(EngineContext context) {
	context_ = context;
}

///=====================================================/// 
/// 登録されているすべての弾を更新
///=====================================================///
void BulletManager::Update() {

	//弾の削除
	RemoveBullet();

	//弾の更新
	for (auto& bullet : bullets_) {

		bullet->Update();
	}
}

///=====================================================/// 
/// すべての弾の座標情報を更新
///=====================================================///
void BulletManager::TransformUpdate() {

	//弾の座標情報のみ更新
	for (auto& bullet : bullets_) {

		bullet->TransformUpdate();
	}
}

///=====================================================/// 
/// 登録されているすべての弾を描画
///=====================================================///
void BulletManager::Draw() {

	//弾の描画
	for (auto& bullet : bullets_) {

		bullet->Draw();
	}
}

///=====================================================/// 
/// 新しい弾を生成して管理リストに追加
///=====================================================///
void BulletManager::AddBullet(Vector3 pos, Vector3 direction, BulletType type) {

	//弾の生成
	std::unique_ptr<BulletBase> bullet = CreateBullet(type);

	//弾の初期化
	bullet->Initialize(context_, pos, direction);

	//弾を管理リストに追加
	bullets_.push_back(std::move(bullet));
}

///=====================================================/// 
/// 指定された種類に応じた弾のインスタンスを生成
///=====================================================///
std::unique_ptr<BulletBase> BulletManager::CreateBullet(BulletType type) {

	std::unique_ptr<BulletBase> newBullet;

	//弾のタイプによってインスタンスを生成
	switch (type) {
	case BulletType::TANK:

		newBullet = std::make_unique<TankBullet>();
		break;
	case BulletType::JET:

		newBullet = std::make_unique<JetBullet>();
		break;
	case BulletType::ENEMY:

		newBullet = std::make_unique<EnemyBullet>();
		break;
	}

	return std::move(newBullet);
}

///=====================================================/// 
/// 削除対象の弾をリストから削除します。
///=====================================================///
void BulletManager::RemoveBullet() {

	//弾の削除
	bullets_.remove_if([](std::unique_ptr<BulletBase>& bullet) {

		//弾が死んでいるかチェック
		if (bullet->IsDead()) {

			return true;
		}

		return false;
		});
}