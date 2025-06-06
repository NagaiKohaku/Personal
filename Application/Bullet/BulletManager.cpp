#include "BulletManager.h"

#include "Bullet/PlayerBullet.h"

void BulletManager::Initialize() {
}

void BulletManager::Update() {

	//弾の削除
	bullets_.remove_if([](std::unique_ptr<BulletBase>& bullet) {

		if (bullet->IsDead()) {

			bullet.reset();

			return true;
		}

		return false;

		});

	//弾の更新
	for (auto& bullet : bullets_) {
		bullet->Update();
	}
}

void BulletManager::Draw() {

	//弾の描画
	for (auto& bullet : bullets_) {
		bullet->Draw();
	}
}

void BulletManager::AddBullet(Vector3 pos) {

	//弾の生成
	std::unique_ptr<BulletBase> bullet = std::make_unique<PlayerBullet>();

	//弾の初期化
	bullet->Initialize(pos);

	//弾を追加
	bullets_.push_back(std::move(bullet));
}