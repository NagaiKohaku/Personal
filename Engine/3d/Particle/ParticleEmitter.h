#pragma once

#include "Base/LayerType.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Matrix4x4.h"
#include "Math/WorldTransform.h"
#include "Math/AABB.h"
#include "3d/Primitive/PrimitiveBase.h"

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

	struct Parameter {
		Vector3 startNum;
		Vector3 startRandomRange;
		Vector3 endNum;
		Vector3 endRandomRange;
		Vector3 velocity;
		Vector3 velocityRandomRange;
		Vector3 acceleration;
		Vector3 accelerationRandomRange;
	};

	struct ColorParameter {
		Vector4 startColor;
		Vector4 startRandomRange;
		Vector4 endColor;
		Vector4 endRandomRange;
		Vector4 velocity;
		Vector4 velocityRandomRange;
		Vector4 acceleration;
		Vector4 accelerationRandomRange;
	};

	struct ParticleParameter {
		Vector3 startNum;
		Vector3 endNum;
		Vector3 velocity;
		Vector3 acceleration;
	};

	struct ParticleColorParameter {
		Vector4 startColor;
		Vector4 endColor;
		Vector4 velocity;
		Vector4 acceleration;
	};

	enum State {
		START,
		VELOCITY,
		EASING
	};

	enum EasingState {
		LINEAR,
		EASE_IN,
		EASE_OUT
	};

	enum PrimitiveType {
		PLANE,
		RING
	};

	//パーティクル
	struct Particle {
		WorldTransform transform;
		Vector4 color;
		ParticleParameter positionPara;
		ParticleParameter rotationPara;
		ParticleParameter scalePara;
		ParticleColorParameter colorPara;
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
	void Initialize(const std::string& fileName, Camera* camera);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(LayerType layer);

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

	void ExportEmitterData(const std::string& fileName);

	void ImportEmitterData(const std::string& fileName);

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

	std::unique_ptr<PrimitiveBase> GetPrimitiveType(PrimitiveType primitiveType);

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

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	/// === 他クラスからの借り物 === ///

	//DirectC基底
	DirectXCommon* directXCommon_;

	//テクスチャマネージャー
	TextureManager* textureManager_;

	//SRVマネージャー
	SrvManager* srvManager_;

	//パーティクルマネージャー
	ParticleManager* particleManager_;

	//デフォルトカメラ
	Camera* defaultCamera_;

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

	//SRVインデックス
	uint32_t srvIndex_;

	/// === パーティクル情報 === ///

	//プリミティブ
	std::unique_ptr<PrimitiveBase> primitive_;

	//マテリアルデータ
	MaterialData material_;

	//インスタンス数
	uint32_t numInstance_;

	//パーティクルリスト
	std::list<Particle> particles_;

	/// === エミッター情報 === ///

	//エミッター名
	std::string name_;

	//テクスチャ名
	std::string textureFileName_;

	//プリミティブタイプ
	PrimitiveType primitiveType_;

	//ワールドトランスフォーム
	WorldTransform emitterWorldTransform_;

	//座標
	Parameter positionParameter_;

	//回転角
	Parameter rotationParameter_;

	//スケール
	Parameter scaleParameter_;

	//色
	ColorParameter colorParameter_;

	//パーティクルの生存時間
	float particleLifeTime_;

	//生成間隔
	float emitFrequency_;

	//生成数
	int32_t emitCount_;

	//生成する最大数
	int32_t emitMaxCount_;

	//生成タイマー
	float emitTimer_;

	//加速場のフラグ
	bool useAccelerationField_;

	//加速場
	AccelerationField accelerationField_;

};