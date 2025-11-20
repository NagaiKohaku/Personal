#include "BulletManager.h"

#include "Bullet/TankBullet.h"
#include "Bullet/JetBullet.h"
#include "Bullet/EnemyBullet.h"

///=====================================================/// 
/// 初期化
///=====================================================///
void BulletManager::Initialize() {
}

///=====================================================/// 
/// 更新
///=====================================================///
void BulletManager::Update() {

	//弾の削除
	bullets_.remove_if([](std::unique_ptr<BulletBase>& bullet) {

		//弾が死んでいるかチェック
		if (bullet->IsDead()) {

			return true;
		}

		return false;
		});

	//弾の更新
	for (auto& bullet : bullets_) {
		bullet->Update();
	}
}

void BulletManager::TransformUpdate() {

	//弾の座標のみ更新
	for (auto& bullet : bullets_) {

		bullet->TransformUpdate();
	}
}

///=====================================================/// 
/// 描画
///=====================================================///
void BulletManager::Draw() {

	//弾の描画
	for (auto& bullet : bullets_) {

		bullet->Draw();
	}
}

///=====================================================/// 
/// 弾の追加
///=====================================================///
void BulletManager::AddBullet(Vector3 pos, Vector3 direction, BULLETTYPE type) {

	//弾の生成
	std::unique_ptr<BulletBase> bullet = CreateBullet(type);

	//弾の初期化
	bullet->Initialize(pos,direction);

	//弾を追加
	bullets_.push_back(std::move(bullet));
}

///=====================================================/// 
/// 弾インスタンスの生成
///=====================================================///
std::unique_ptr<BulletBase> BulletManager::CreateBullet(BULLETTYPE type) {

	std::unique_ptr<BulletBase> newBullet;

	//弾のタイプによってインスタンスを生成
	switch (type) {
	case BulletManager::TANK:

		newBullet = std::make_unique<TankBullet>();
		break;
	case BulletManager::JET:

		newBullet = std::make_unique<JetBullet>();
		break;
	case BulletManager::ENEMY:

		newBullet = std::make_unique<EnemyBullet>();
		break;
	}

	return std::move(newBullet);
}