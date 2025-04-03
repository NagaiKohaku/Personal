#include "ParticleManager.h"

#include "Base/DirectXCommon.h"
#include "Base/SrvManager.h"
#include "2d/Sprite/TextureManager.h"
#include "3d/Camera/Camera.h"
#include "3d/Particle/ParticleGroup.h"
#include "3d/Particle/ParticleCommon.h"

#include "Math/MakeMatrixMath.h"

#include "Other/Log.h"

#include "random"
#include "numbers"

///=====================================================/// 
/// シングルトンインスタンス
///=====================================================///
ParticleManager* ParticleManager::GetInstance() {
	static ParticleManager instance;
	return &instance;
}

///=====================================================/// 
/// 初期化処理
///=====================================================///
void ParticleManager::Initialize() {

	//DirectX基底のインスタンスを取得
	directXCommon_ = DirectXCommon::GetInstance();

	//テクスチャマネージャーのインスタンスを取得
	textureManager_ = TextureManager::GetInstance();

	//SRVマネージャーのインスタンスを取得
	srvManager_ = SrvManager::GetInstance();

	particleGroups.reserve(srvManager_->kMaxSRVCount_);

}

///=====================================================/// 
/// 更新処理
///=====================================================///
void ParticleManager::Update() {

	//すべてのパーティクルグループの処理をする
	for (std::unordered_map<std::string, ParticleGroup>::iterator particleGroupIterator = particleGroups.begin();
		particleGroupIterator != particleGroups.end();) {

		particleGroupIterator->second.Update();

		++particleGroupIterator;
	}

	CheckCollisionAccelerationField();
}

///=====================================================/// 
/// 描画処理
///=====================================================///
void ParticleManager::Draw() {

	ParticleCommon::GetInstance()->CommonDrawSetting();

	//すべてのパーティクルグループの処理をする
	for (std::unordered_map<std::string, ParticleGroup>::iterator particleGroupIterator = particleGroups.begin();
		particleGroupIterator != particleGroups.end();) {

		particleGroupIterator->second.Draw();

		++particleGroupIterator;
	}

}

///=====================================================/// 
/// パーティクルグループの生成
///=====================================================///
void ParticleManager::CreateParticleGroup(const std::string name, const std::string textureFilePath) {

	//登録済みの名前かチェック
	if (particleGroups.contains(name)) {
		//登録済みなら早期return
		return;
	}

	//あたらしいパーティクルグループを生成
	ParticleGroup& particleGroup = particleGroups[name];

	particleGroup.Initialize("Resource/Sprite/Particle/" + textureFilePath, defaultCamera_);
}

///=====================================================/// 
/// 発生
///=====================================================///
void ParticleManager::Emit(const std::string name, const Vector3& translate, const AABB& area, const Vector3& minVelocity, const Vector3& maxVelocity, float minTime, float maxTime, bool useRandomColor, uint32_t count) {

	//登録済みの名前かチェック
	if (particleGroups.contains(name)) {

		particleGroups[name].Emit(translate, area, minVelocity, maxVelocity, minTime, maxTime, useRandomColor, count);
	}

}

///=====================================================/// 
/// 加速場との接触判定
///=====================================================///
void ParticleManager::CheckCollisionAccelerationField() {

	//すべてのパーティクルグループの処理をする
	for (std::unordered_map<std::string, ParticleGroup>::iterator particleGroupIterator = particleGroups.begin();
		particleGroupIterator != particleGroups.end();) {

		particleGroupIterator->second.CheckCollisionAccelerationField();

		++particleGroupIterator;
	}
}

///=====================================================/// 
/// 加速場のセッター
///=====================================================///
void ParticleManager::SetAcceleration(const std::string name, const Vector3& acceleration, const AABB& area) {

	//登録済みの名前かチェック
	if (particleGroups.contains(name)) {

		particleGroups[name].SetAccelerationField(acceleration, area);
	}
}