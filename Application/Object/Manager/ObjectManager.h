#pragma once

#include <memory>
#include <list>

class Player;
class Enemy;
class BombEnemy;
class EnemyManager;

class ObjectManager {

public:

	static ObjectManager* GetInstance();

	void Initialize();

	void Update();

	void Draw();

	void SpawnPlayer();

	void SpawnEnemy();

	void SpawnBombEnemy();

	void ClearAll();

private:

	std::unique_ptr<Player> player_;

	std::list<std::unique_ptr<Enemy>> enemies_;

	std::list<std::unique_ptr<BombEnemy>> bombEnemy_;

	int killCount_ = 0;

public:

	Player* GetPlayer() { return player_.get(); }

	std::list<Enemy*> GetEnemies();

	std::list<BombEnemy*> GetBombEnemies();

	int GetKillCount() { return killCount_; }

	void AddKillCount() { killCount_++; }
};