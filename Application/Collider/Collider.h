#pragma once

#include "3d/Object/Object3D.h"
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
		BULLET,
		WALL,
		ITEM,
		NONE
	};

public:

	virtual void Initialize(WorldTransform& parent);

	virtual void Update();

	virtual void Draw();

public:

	WorldTransform& GetWorldTransform() { return worldTransform_; }

	Tag GetTag() const { return tag_; }

	Tag GetHitTag() const { return hitTag_; }

	bool GetIsActive() const { return isActive_; }

	void SetTag(Tag tag) { tag_ = tag; }

	void SetHitTag(Tag hitTag) { hitTag_ = hitTag; }

	void SetIsDraw(bool isDraw) { isDraw_ = isDraw; }

	void SetIsActive(bool isActive) { isActive_ = isActive; }

protected:

	virtual void CreateDebugLines();

protected:

	Tag tag_ = NONE;

	Tag hitTag_ = NONE;

	WorldTransform worldTransform_;

	std::vector<std::unique_ptr<DebugLine>> debugLines_;

	Vector4 defaultColor_;

	Vector4 hitColor_;

	bool isDraw_ = false;

	bool isActive_ = true;
};