#include "GroundBullet.h"

#include "Object/Ground/GroundManager.h"

using namespace MyEngine;

void GroundBullet::Update() {

	timer_ += 1.0f / 60.0f;

	BulletBase::Update();

	if (object_->GetWorldTransform().translate_.y <= 0.0f) {

		CreateDamageGround();
	}

	if (timer_ >= lifeTime_) {

		isDead_ = true;
	}

}

void GroundBullet::IsCollision() {

}

void GroundBullet::Draw() {

	BulletBase::Draw();

}

void GroundBullet::CreateDamageGround() {

	if (!beenCreated_) {

		Vector3 hitPos = object_->GetWorldTransform().translate_;

		GroundManager::GetInstance()->CreateDamageGround(hitPos);

		beenCreated_ = true;
	}
}