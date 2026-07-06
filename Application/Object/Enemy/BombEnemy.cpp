#include "BombEnemy.h"

#include "Object/Manager/ObjectManager.h"
#include "Object/Bullet/Core/BulletManager.h"

#include <numbers>

using namespace MyEngine;

void BombEnemy::Initialize(BulletManager* bulletPtr, MyEngine::Vector3 pos) {

	bulletManager_ = bulletPtr;

	object_ = std::make_unique<Object3D>();

	object_->Initialize();

	object_->SetModel("Oni");

	object_->GetWorldTransform().translate_ = pos;

	object_->GetWorldTransform().rotate_ = Vector3(0.0f, std::numbers::pi_v<float>, 0.0f);

	object_->TransformUpdate();
}

void BombEnemy::Update() {

	timer_ += 1.0f / 60.0f;

	object_->GetWorldTransform().translate_.z -= speed_;

	if (timer_ >= coolTime_) {

		if (shootCount_ <= maxCount_) {

			bulletManager_->AddBullet(
				object_->GetWorldTransform().translate_,
				Vector3(0.0f, -1.0f, -1.0f),
				BulletManager::BulletType::GROUND
			);

			shootCount_++;
		}
	}

	if (object_->GetWorldTransform().translate_.z <= -100.0f) {
		canRemove_ = true;
	}

	object_->Update();
}

void BombEnemy::Draw() {

	object_->Draw(LayerType::OBJECT);
}