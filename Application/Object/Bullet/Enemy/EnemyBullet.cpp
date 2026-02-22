#include "EnemyBullet.h"

using namespace MyEngine;

///=====================================================/// 
/// 他オブジェクトと接触している場合の処理を行う
///=====================================================///
void EnemyBullet::IsCollision() {

	//接触状態であれば
	if (collider_->GetIsTrigger()) {

		//接触相手のタグがPLAYERであれば
		if (collider_->CheckHitTag(Collider::Tag::PLAYER)) {

			isDead_ = true;
		}
	}
}