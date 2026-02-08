#include <Object/Manager/ObjectManager.h>

#include <Object/Player/Player.h>
#include <Object/Enemy/Enemy.h>
#include <Object/Enemy/EnemyManager.h>

using namespace MyEngine;

ObjectManager* ObjectManager::GetInstance() {
	static ObjectManager instance;
	return &instance;
}

void ObjectManager::Initialize() {
}

void ObjectManager::Update() {

	//エネミーの削除
	enemies_.remove_if([](const std::unique_ptr<Enemy>& enemy) {
		if (enemy->GetIsRemove()) {
			return true;
		}
		return false;
		});

	//if (player_) {
	//	player_->Update();
	//}

	//for(auto& enemy : enemies_) {

	//	enemy->Update();
	//}
}

void ObjectManager::Draw() {

	//if (player_) {
	//	player_->Draw();
	//}

	//for (auto& enemy : enemies_) {

	//	enemy->Draw();
	//}
}

void ObjectManager::SpawnPlayer() {
	player_ = std::make_unique<Player>();
}

void ObjectManager::SpawnEnemy() {
	enemies_.push_back(std::make_unique<Enemy>());
}

void ObjectManager::ClearAll() {
	player_.reset();
	enemies_.clear();
	killCount_ = 0;
}

std::list<Enemy*> ObjectManager::GetEnemies() {

	std::list<Enemy*> enemyList;

	for (auto& enemy : enemies_) {
		enemyList.push_back(enemy.get());
	}

	return enemyList;
}