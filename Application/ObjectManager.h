#pragma once

#include <memory>
#include <list>

class Player;
class Enemy;
class Boss;
class EnemyManager;

class ObjectManager {

public:

	static ObjectManager* GetInstance();

	void Initialize();

	void Update();

	void Draw();

	void SpawnPlayer();

	void SpawnEnemy();

	void SpawnBoss();

	void ClearAll();

private:

	std::unique_ptr<Player> player_;

	std::list<std::unique_ptr<Enemy>> enemies_;

	std::unique_ptr<Boss> boss_;

	int killCount_ = 0;

public:

	Player* GetPlayer() { return player_.get(); }

	std::list<Enemy*> GetEnemies();

	Boss* GetBoss() { return boss_.get(); }

	int GetKillCount() { return killCount_; }

	void AddKillCount() { killCount_++; }
};