#pragma once

#include "Reticle.h"

#include "memory"
#include "vector"
#include "list"

class Player;
class Enemy;
class Camera;

class LockOn {

	struct LockOnReticle {
		std::unique_ptr<Reticle> reticle;
		Enemy* targetEnemy;
		Enemy* preTargetEnemy;
		bool isActive;
	};

	struct LockedEnemy {
		Enemy* enemy;
		bool isAssigned;
	};

public:

	void Initialize(Camera* cameraPtr, Player* playerPtr);

	void Update();

	void Draw();

	void AddLockOnEnemy(Enemy* target);

private:

	std::unique_ptr<Reticle> mainReticle_;

	std::list<std::unique_ptr<LockOnReticle>> lockOnReticles_;

	std::list<LockedEnemy> lockedEnemies_;

	Camera* camera_;

	Player* player_;

	//オフセット
	Vector3 offset_;

	//移動範囲
	Vector3 moveRange_;

	//移動強度
	float moveStrength_;

	//最大ロックオン数
	int maxLockOnNum_;

public:

	Vector3 GetMainReticlePos() const { return mainReticle_->GetWorldPos(); }

	std::vector<Vector3> GetLockOnReticlePos();

};