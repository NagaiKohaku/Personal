#pragma once

#include "3d/Particle/ParticleGroup.h"

#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Matrix4x4.h"
#include "Math/AABB.h"

#include "d3d12.h"

#include "string"
#include "list"
#include "unordered_map"
#include "random"
#include "wrl.h"

/// === 前方宣言 === ///
class DirectXCommon;

class TextureManager;

class SrvManager;

class Camera;

class ParticleManager {

	///-------------------------------------------/// 
	/// 静的メンバ変数
	///-------------------------------------------///
public:

	//インスタンスの最大数
	static const uint32_t kNumMaxInstance;

	//1フレームで進む秒数
	static const float kDeltaTime;

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// シングルトンインスタンス
	/// </summary>
	/// <returns>インスタンス</returns>
	static ParticleManager* GetInstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// パーティクルグループの生成
	/// </summary>
	/// <param name="name">名前</param>
	/// <param name="textureFilePath">テクスチャパス</param>
	void CreateParticleGroup(const std::string name, const std::string textureFilePath);

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="name">名前</param>
	/// <param name="position">座標</param>
	/// <param name="count">数</param>
	void Emit(const std::string name, const Vector3& translate, const AABB& area, const Vector3& minVelocity, const Vector3& maxVelocity, float minTime, float maxTime, bool useRandomColor, uint32_t count);

	/// <summary>
	/// 加速場との接触判定
	/// </summary>
	void CheckCollisionAccelerationField();

	///-------------------------------------------/// 
	/// セッター・ゲッター
	///-------------------------------------------///
public:

	/// <summary>
	/// デフォルトカメラの設定
	/// </summary>
	/// <param name="camera">カメラ</param>
	void SetDefaultCamera(Camera* camera) { defaultCamera_ = camera; }

	/// <summary>
	/// 加速場のセッター
	/// </summary>
	/// <param name="name">名前</param>
	/// <param name="acceleration">加速度</param>
	void SetAcceleration(const std::string name, const Vector3& acceleration, const AABB& area);

	/// <summary>
	/// デフォルトカメラのゲッター
	/// </summary>
	/// <returns>カメラ</returns>
	Camera* GetDefaultCamera() const { return defaultCamera_; }

	/// <summary>
	/// パーティクルグループのゲッター
	/// </summary>
	/// <returns>パーティクルグループ</returns>
	std::unordered_map<std::string, ParticleGroup> GetParticleGroup() { return particleGroups; }

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//DirectC基底
	DirectXCommon* directXCommon_;

	//テクスチャマネージャー
	TextureManager* textureManager_;

	//SRVマネージャー
	SrvManager* srvManager_;

	//デフォルトカメラ
	Camera* defaultCamera_;

	//パーティクルグループ
	std::unordered_map<std::string, ParticleGroup> particleGroups;

};