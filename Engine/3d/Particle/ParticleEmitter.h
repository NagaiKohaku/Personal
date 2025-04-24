#pragma once

#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Matrix4x4.h"
#include "Math/WorldTransform.h"
#include "Math/AABB.h"
#include "3d/Primitive/PrimitiveBase.h"
#include "3d/Primitive/Plane.h"

#include "d3d12.h"

#include "string"
#include "list"
#include "memory"
#include "wrl.h"

/// === 前方宣言 === ///
class DirectXCommon;

class TextureManager;

class SrvManager;

class Camera;

class ParticleCommon;

class ParticleManager;

class ParticleEmitter {

	///-------------------------------------------/// 
	/// メンバ構造体
	///-------------------------------------------///
private:

	//マテリアル
	struct Material {
		Vector4 color;
		int32_t enableLighting;
		float padding[3];
		Matrix4x4 uvTransform;
	};

	//ローカル情報
	struct Transform {
		Vector3 scale;
		Vector3 rotate;
		Vector3 translate;
	};

	//パーティクル
	struct Particle {
		Transform transform;
		Vector3 velocity;
		Vector4 color;
		float lifeTime;
		float currentTime;
	};

	//パーティクルGPU転送用データ
	struct ParticleForGPU {
		Matrix4x4 WVP;
		Matrix4x4 World;
		Vector4 color;
	};

	//モデルのマテリアルデータ
	struct MaterialData {
		std::string textureFilePath;
		uint32_t textureIndex = 0;
	};

	//加速場
	struct AccelerationField {
		Vector3 acceleration;
		AABB area;
	};

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
	/// 初期化処理
	/// </summary>
	void Initialize(std::string name, const std::string textureFilePath, Camera* camera);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// パーティクルの生成
	/// </summary>
	/// <param name="translate">初期座標</param>
	/// <param name="area">生成範囲</param>
	/// <param name="minVelocity">最小速度</param>
	/// <param name="maxVelocity">最大速度</param>
	/// <param name="minTime">最小時間</param>
	/// <param name="maxTime">最大時間</param>
	/// <param name="useRandomColor">色のランダムフラグ</param>
	/// <param name="count">生成数</param>
	void Emit(
		const Vector3& translate,
		const AABB& area,
		const Vector3& minVelocity,
		const Vector3& maxVelocity,
		float minTime,
		float maxTime,
		bool useRandomColor,
		uint32_t count
	);

	/// <summary>
	/// パーティクルの生成
	/// </summary>
	/// <param name="translate">初期座標</param>
	/// <param name="area">生成範囲</param>
	/// <param name="minVelocity">最小速度</param>
	/// <param name="maxVelocity">最大速度</param>
	/// <param name="minTime">最小時間</param>
	/// <param name="maxTime">最大時間</param>
	/// <param name="useRandomColor">色のランダムフラグ</param>
	/// <param name="count">生成数</param>
	void EmitPlane(const Vector3& translate, const AABB& area, const Vector3& minVelocity, const Vector3& maxVelocity, float minTime, float maxTime, bool useRandomColor, uint32_t count);

	/// <summary>
	/// 加速場との接触判定
	/// </summary>
	void CheckCollisionAccelerationField();

	/// <summary>
	/// 接触判定
	/// </summary>
	/// <param name="aabb">AABB</param>
	/// <param name="point"></param>
	/// <returns>フラグ</returns>
	bool IsCollision(const AABB& aabb, const Vector3& point);

	///-------------------------------------------/// 
	/// ゲッター・セッター
	///-------------------------------------------///
public:

	/// <summary>
	/// 加速場のセッター
	/// </summary>
	/// <param name="acceleration"></param>
	void SetAccelerationField(const Vector3& acceleration, const AABB& area);

	WorldTransform& GetWorldTransform() { return emitterWorldTransform_; }

	///-------------------------------------------/// 
	/// クラス内処理関数
	///-------------------------------------------///
private:

	/// <summary>
	/// 新しいパーティクルの生成
	/// </summary>
	/// <param name="translate">座標</param>
	/// <param name="area">生成範囲</param>
	/// <param name="minVelocity">最小速度</param>
	/// <param name="maxVelocity">最大速度</param>
	/// <param name="minTime">最小生成時間</param>
	/// <param name="maxTime">最大生成時間</param>
	/// <param name="useRandomColor">色のランダムフラグ</param>
	/// <returns>パーティクル</returns>
	Particle MakeNewParticle(const Vector3& translate, const AABB& area, const Vector3& minVelocity, const Vector3& maxVelocity, float minTime, float maxTime, bool useRandomColor);

	/// <summary>
	/// 新しいパーティクルの生成
	/// </summary>
	/// <param name="translate">座標</param>
	/// <param name="area">生成範囲</param>
	/// <param name="minVelocity">最小速度</param>
	/// <param name="maxVelocity">最大速度</param>
	/// <param name="minTime">最小生成時間</param>
	/// <param name="maxTime">最大生成時間</param>
	/// <param name="useRandomColor">色のランダムフラグ</param>
	/// <returns>パーティクル</returns>
	Particle MakeNewPlaneParticle(const Vector3& translate, const AABB& area, const Vector3& minVelocity, const Vector3& maxVelocity, float minTime, float maxTime, bool useRandomColor);

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

	//パーティクルマネージャー
	ParticleManager* particleManager_;

	//ワールドトランスフォーム
	WorldTransform emitterWorldTransform_;

	//SRVインデックス
	uint32_t srvIndex_;

	//インスタンス数
	uint32_t numInstance_;

	//加速場のフラグ
	bool useAccelerationField_;

	//アンカーポイント
	Vector3 anchorPoint_;

	//マテリアルデータ
	MaterialData material_;

	//加速場
	AccelerationField accelerationField_;

	//パーティクルリスト
	std::list<Particle> particles_;

	std::unique_ptr<PrimitiveBase> primitive_;

	/// === バッファリソース === ///

	//マテリアルリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_ = nullptr;

	//インスタンシングリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> instancingResource_;

	/// === バッファリソース内のデータを指すポインタ === ///

	//マテリアルデータ
	Material* materialData_ = nullptr;

	//インスタンシングデータ
	ParticleForGPU* instancingData_;

};