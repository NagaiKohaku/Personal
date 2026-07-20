#pragma once

#include "3d/Object/Object3D.h"

class BulletManager;

class BombEnemy {

public:

	void Initialize(BulletManager* bulletPtr, MyEngine::Vector3 pos);

	void Update();

	void Draw();

private:

	std::unique_ptr<MyEngine::Object3D> object_;

	BulletManager* bulletManager_;

	float speed_ = 3.0f;

	float timer_ = 0.0f;

	float coolTime_ = 1.0f;

	float turnRangeX_ = 15.0f;

	float moveDirectionX_ = 1.0f;

	int shootCount_ = 0;

	int maxCount_ = 3;

	//削除可能フラグ
	bool canRemove_;

	//削除中フラグ
	bool isRemove_;

public:

	/// <summary>
	/// 削除中フラグのセッター
	/// </summary>
	/// <param name="flag">フラグ</param>
	void SetIsRemove(bool flag) { isRemove_ = flag; }

	/// <summary>
	/// 削除可能フラグのゲッター
	/// </summary>
	/// <returns>フラグ</returns>
	bool GetCanRemove() { return canRemove_; }

	/// <summary>
	/// 削除中フラグのゲッター
	/// </summary>
	/// <returns>フラグ</returns>
	bool GetIsRemove() { return isRemove_; }

};