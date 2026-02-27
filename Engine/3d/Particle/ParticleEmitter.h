#pragma once

#include "Base/LayerType.h"
#include <Math/Vector/Vector2.h>
#include <Math/Vector/Vector3.h>
#include <Math/Vector/Vector4.h>
#include <Math/Matrix/Matrix4x4.h>
#include <Math/Transform/WorldTransform.h>
#include <Math/Shape/AABB.h>
#include "3d/Mesh/MeshBase.h"
#include "3d/Model/Model.h"
#include <Base/Input.h>
#include <Base/Renderer.h>


#include "d3d12.h"

#include "string"
#include "list"
#include "memory"
#include "wrl.h"

namespace MyEngine {

	/// === 前方宣言 === ///
	class DirectXCommon;

	class TextureManager;

	class SRVManager;

	class Camera;

	class ParticleCommon;

	class ParticleManager;

	/// <summary>
	/// 個別のパーティクル生成・管理・描画を行うエミッタークラスです。
	/// </summary>
	class ParticleEmitter {

		///-------------------------------------------/// 
		/// 静的メンバ変数
		///-------------------------------------------///
	public:

		//インスタンスの最大数
		static const uint32_t kNumMaxInstance;

		//1フレームで進む秒数
		static const float kDeltaTime;

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

		//エミッターのパラメータ
		struct EmitterParameter {
			Vector3 startNum;
			Vector3 startRandomRange;
			Vector3 endNum;
			Vector3 endRandomRange;
			Vector3 velocity;
			Vector3 velocityRandomRange;
			Vector3 acceleration;
			Vector3 accelerationRandomRange;
		};

		//エミッターの色のパラメータ
		struct EmitterColorParameter {
			Vector4 startColor;
			Vector4 startRandomRange;
			Vector4 endColor;
			Vector4 endRandomRange;
			Vector4 velocity;
			Vector4 velocityRandomRange;
			Vector4 acceleration;
			Vector4 accelerationRandomRange;
		};

		//パーティクルのパラメータ
		struct ParticleParameter {
			Vector3 startNum;
			Vector3 endNum;
			Vector3 velocity;
			Vector3 acceleration;
		};

		//パーティクルの色のパラメータ
		struct ParticleColorParameter {
			Vector4 startColor;
			Vector4 endColor;
			Vector4 velocity;
			Vector4 acceleration;
		};

		//更新状態
		enum class UpdateState {
			START,
			VELOCITY,
			EASING
		};

		//イージング状態
		enum class EasingState {
			LINEAR,
			EASE_IN,
			EASE_OUT,
			EASE_INOUT,
		};

		//メッシュの種類
		enum class MeshType {
			PLANE,
			RING,
			CYLINDER,
			BALL
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

		///-------------------------------------------/// 
		/// メンバ関数
		///-------------------------------------------///
	public:

		/// <summary>
		/// パーティクルエミッターのデストラクタ
		/// </summary>
		~ParticleEmitter();

		/// <summary>
		/// パーティクルエミッターを初期化します。
		/// </summary>
		void Initialize(const std::string& groupName, const std::string& fileName, ParticleCommon* particleCommonPtr, Camera* cameraPtr, Input* inputPtr, Renderer* rendererPtr);

		/// <summary>
		/// パーティクルエミッターの更新処理を行います。
		/// </summary>
		void Update();

		/// <summary>
		/// パーティクルエミッターの描画処理を行います。
		/// </summary>
		void Draw(LayerType layer);

		/// <summary>
		/// ImGuiの表示
		/// </summary>
		void ImGui();

		/// <summary>
		/// パーティクルの生成を開始します。
		/// </summary>
		void Emit();

		/// <summary>
		/// エミッターの設定情報をJSONファイルに書き出します。
		/// </summary>
		/// <param name="groupName">エミッターが所属するグループ名。</param>
		void ExportEmitterData(const std::string& groupName);

		/// <summary>
		/// JSONファイルからエミッターの設定情報を読み込みます。
		/// </summary>
		/// <param name="groupName">エミッターが所属するグループ名。</param>
		/// <param name="fileName">読み込むエミッターファイル名</param>
		void ImportEmitterData(const std::string& groupName, const std::string& fileName);

		///-------------------------------------------/// 
		/// ゲッター・セッター
		///-------------------------------------------///
	public:

		/// <summary>
		/// トランスフォームの取得
		/// </summary>
		/// <returns>トランスフォーム</returns>
		WorldTransform& GetWorldTransform() { return emitterWorldTransform_; }

		/// <summary>
		/// エミッター名の取得
		/// </summary>
		/// <returns>エミッター名</returns>
		std::string GetName() { return name_; }

		/// <summary>
		/// トランスフォームの設定
		/// </summary>
		/// <param name="worldTransform">トランスフォーム</param>
		void SetWorldTransform(WorldTransform worldTransform) { emitterWorldTransform_ = worldTransform; }

		/// <summary>
		/// テクスチャリストの設定
		/// </summary>
		/// <param name="list">テクスチャリスト</param>
		void SetTextureList(std::vector<std::string> list) { textureList_ = list; }

		/// <summary>
		/// 発生フラグの設定
		/// </summary>
		/// <param name="flag">発生フラグ</param>
		void SetIsEmit(bool flag) { isEmit_ = flag; }

		///-------------------------------------------/// 
		/// クラス内処理関数
		///-------------------------------------------///
	private:

		/// <summary>
		/// 新しいパーティクルを生成し、初期パラメータを設定します。
		/// </summary>
		/// <returns>初期化済みのParticle構造体</returns>
		Particle MakeNewParticle();

		/// <summary>
		/// パーティクルの各パラメータ（位置・回転・スケールなど）を更新します。
		/// </summary>
		/// <param name="num">更新値</param>
		/// <param name="parameter">パラメータ構造体</param>
		/// <param name="updateState">更新方法を指定する列挙型</param>
		/// <param name="easingState">補間方法を指定する列挙型</param>
		/// <param name="easingStrength">補間強度</param>
		/// <param name="currentTime">パーティクルの経過時間</param>
		/// <param name="lifeTime">パーティクルの寿命</param>
		void UpdateParameter(Vector3& num, ParticleParameter& parameter, UpdateState& updateState, EasingState& easingState, float& easingStrength, float& currentTime, float& lifeTime);

		/// <summary>
		/// パラメータのImGui表示
		/// </summary>
		/// <param name="labelName"></param>
		/// <param name="parameter"></param>
		/// <param name="updateState"></param>
		/// <param name="easingState"></param>
		/// <param name="easingStrength"></param>
		void ImGuiParameter(std::string labelName, EmitterParameter& parameter, UpdateState& updateState, EasingState& easingState, float& easingStrength);

		/// <summary>
		/// ImGuiなどで使用するユニークラベル名を生成します。
		/// </summary>
		/// <param name="labelName">ラベルのベースとなる文字列</param>
		/// <param name="label">追加で付与する識別用文字列（C文字列）</param>
		/// <returns>「##ベース名+識別子」という形式の文字列を返す</returns>
		std::string CreateLabelName(std::string labelName, const char* label);

		///-------------------------------------------/// 
		/// メンバ変数
		///-------------------------------------------///
	private:

		/// === 他クラスからの借り物 === ///

		ParticleCommon* particleCommon_;

		//DirectX基底
		DirectXCommon* directXCommon_;

		//テクスチャマネージャー
		TextureManager* textureManager_;

		//SRVマネージャー
		SRVManager* srvManager_;

		//パーティクルマネージャー
		ParticleManager* particleManager_;

		//デフォルトカメラ
		Camera* defaultCamera_;

		Input* input_;

		Renderer* renderer_;

		/// === バッファリソース === ///

		//マテリアルリソース
		Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_ = nullptr;

		//インスタンシングリソース
		Microsoft::WRL::ComPtr<ID3D12Resource> instancingResource_;

		/// === リソースデータ === ///

		//マテリアルデータ
		Material* materialData_ = nullptr;

		//インスタンシングデータ
		ParticleForGPU* instancingData_;

		//SRVインデックス
		uint32_t srvIndex_;

		/// === パーティクル情報 === ///

		//モデル
		std::unique_ptr<Model> model_;

		//インスタンス数
		uint32_t numInstance_;

		//パーティクルリスト
		std::list<Particle> particles_;

		/// === エミッター情報 === ///

		//エミッターのディレクトリパス
		std::string directoryPath_;

		//テクスチャリスト
		std::vector<std::string> textureList_;

		//エミッター名
		std::string name_;

		//モデル名
		std::string modelName_;

		//モデルファイル名
		std::string modelFileName_;

		//テクスチャ名
		std::string textureFileName_;

		//ワールドトランスフォーム
		WorldTransform emitterWorldTransform_;

		/// === 座標パラメータ === ///

		//座標パラメータ
		EmitterParameter positionParameter_;

		//座標の更新状態
		UpdateState positionUpdateState_;

		//座標のイージング状態
		EasingState positionEasingState_;

		//座標のイージング強度
		float positionEasingStrength_;

		/// === 角度パラメータ === ///

		//角度パラメータ
		EmitterParameter rotationParameter_;

		//角度の更新状態
		UpdateState rotationUpdateState_;

		//角度のイージング状態
		EasingState rotationEasingState_;

		//角度のイージング強度
		float rotationEasingStrength_;

		/// === スケールパラメータ === ///

		//スケールパラメータ
		EmitterParameter scaleParameter_;

		//スケールの更新状態
		UpdateState scaleUpdateState_;

		//スケールのイージング状態
		EasingState scaleEasingState_;

		//スケールのイージング強度
		float scaleEasingStrength_;

		/// === 色パラメータ === ///

		//色パラメータ
		EmitterColorParameter colorParameter_;

		//色の更新状態
		UpdateState colorUpdateState_;

		//色のイージング状態
		EasingState colorEasingState_;

		//色のイージング強度
		float colorEasingStrength_;

		/// === 生成パラメータ === ///

		//パーティクルの生存時間
		float particleLifeTime_;

		//パーティクルの生存時間のランダム幅
		float particleLifeTimeRandomRange_;

		//生成間隔
		float emitFrequency_;

		//生成する最大数
		int32_t emitMaxCount_;

		//ループフラグ
		bool isLoop_;

		//生成数無限フラグ
		bool isInfinity_;

		//ビルボードフラグ
		bool isBillboard_;

		//生成数
		int32_t emitCount_;

		//生成タイマー
		float emitTimer_;

		//生成フラグ
		bool isEmit_;

		//アクティブフラグ
		bool isActive_;
	};
}