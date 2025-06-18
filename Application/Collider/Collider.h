#pragma once

#include "3d/Object/Object3D.h"
#include "3d/Object/DebugObject3D.h"
#include "3d/Object/DebugLine.h"

#include "Math/WorldTransform.h"
#include "Math/AABB.h"
#include "Math/Sphere.h"

#include "vector"
#include "memory"

class Collider {

public:

	enum Tag {
		PLAYER,
		ENEMY,
		PLAYERBULLET,
		ENEMYBULLET,
		WALL,
		ITEM,
		NONE
	};

public:

	virtual void Initialize(WorldTransform* parent);

	virtual void Update();

	virtual void Draw();

public:

	WorldTransform& GetWorldTransform() { return worldTransform_; }

	Tag GetTag() const { return tag_; }

	Tag GetHitTag() const { return hitTag_; }

	bool GetIsActive() const { return isActive_; }

	bool GetIsCollision() const { return isCollision_; }

	bool GetIsTrigger() const { return isTrigger_; }

	void SetTag(Tag tag) { tag_ = tag; }

	void SetHitTag(Tag hitTag) { hitTag_ = hitTag; }

	void SetIsDraw(bool isDraw) { isDraw_ = isDraw; }

	void SetIsActive(bool isActive) { isActive_ = isActive; }

protected:

	Tag tag_ = NONE;

	Tag hitTag_ = NONE;

	Tag hitTagBefore_ = NONE;

	WorldTransform worldTransform_;

	WorldTransform* parentTransform_;

	std::unique_ptr<DebugObject3D> debugObject_;

	Vector4 defaultColor_;

	Vector4 hitColor_;

	bool isDraw_ = false;

	bool isActive_ = true;

	bool isCollision_ = false;

	bool isTrigger_ = false;
};