#pragma once

#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Matrix4x4.h"
#include "Math/AABB.h"

#include "d3d12.h"

#include "string"
#include "list"
#include "random"
#include "wrl.h"

/// === 前方宣言 === ///
class DirectXCommon;

class TextureManager;

class SrvManager;

class Camera;

class ParticleCommon;

///=====================================================/// 
/// パーティクルグループ
///=====================================================///
class ParticleGroup {

	///-------------------------------------------/// 
	/// メンバ構造体
	///-------------------------------------------///
private:

	//頂点データ
	struct VertexData {
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};

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
	void Initialize(const std::string textureFilePath, Camera* camera);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="name">名前</param>
	/// <param name="position">座標</param>
	/// <param name="count">数</param>
	void Emit(const Vector3& translate, const AABB& area, const Vector3& minVelocity, const Vector3& maxVelocity, float minTime, float maxTime, bool useRandomColor, uint32_t count);

	void CheckCollisionAccelerationField();

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

	///-------------------------------------------/// 
	/// クラス内処理関数
	///-------------------------------------------///
private:

	/// <summary>
	/// 新しいパーティクルの生成
	/// </summary>
	/// <param name="translate">座標</param>
	/// <returns>パーティクル</returns>
	Particle MakeNewParticle(const Vector3& translate, const AABB& area, const Vector3& minVelocity, const Vector3& maxVelocity, float minTime, float maxTime, bool useRandomColor);

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

	//ランダム
	std::mt19937 randomEngine_;

	MaterialData material_;

	std::list<Particle> particles_;

	uint32_t srvIndex_;

	uint32_t numInstance_;

	/// === バッファリソース === ///

	//頂点リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_ = nullptr;

	//頂点番号リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> IndexResource_ = nullptr;

	//マテリアルリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> instancingResource_;

	/// === バッファリソース内のデータを指すポインタ === ///

	//頂点データ
	VertexData* vertexData_ = nullptr;

	//頂点番号データ
	uint32_t* indexData_ = nullptr;

	//マテリアルデータ
	Material* materialData_ = nullptr;

	ParticleForGPU* instancingData_;

	/// === バッファビュー === ///

	//頂点バッファービュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

	//頂点番号バッファビュー
	D3D12_INDEX_BUFFER_VIEW indexBufferView_;

	AccelerationField accelerationField_;

	bool useAccelerationField_;

};