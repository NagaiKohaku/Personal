#pragma once

#include "Base/LayerType.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Matrix4x4.h"
#include "Math/WorldTransform.h"
#include "Math/AABB.h"
#include "3d/Mesh/MeshBase.h"
#include "3d/Model/Model.h"
#include "3d/Collider/SphereCollider.h"


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

/// <summary>
/// 個別のパーティクル生成・管理・描画を行うエミッタークラスです。
/// </summary>
/// <remarks>
/// - パーティクルエミッターは、指定された位置・回転・スケール・色に基づきパーティクルを生成します。  
/// - JSONファイルからエミッター情報を読み込み、ワールドトランスフォームやパラメータ（位置・回転・スケール・色）を設定します。  
/// - 発生・停止フラグの管理により、パーティクルの生成を制御できます。  
/// - エミッター単位でのJSON保存・読み込みにより、パーティクル設定の再利用や管理が可能です。  
/// </remarks>
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
	enum UpdateState {
		START,
		VELOCITY,
		EASING
	};

	//イージング状態
	enum EasingState {
		LINEAR,
		EASE_IN,
		EASE_OUT,
		EASE_INOUT,
	};

	//メッシュの種類
	enum MeshType {
		PLANE,
		RING,
		CYLINDER,
		BALL
	};

	//パーティクル
	struct Particle {
		WorldTransform transform;
		std::unique_ptr<SphereCollider> collider;
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
	/// <remarks>
	/// - エミッターが使用していたGPUリソースの管理番号を解放します。  
	/// - SRV管理クラスに対して、使用していたインデックスを再利用可能として登録します。  
	/// </remarks>
	~ParticleEmitter();

	/// <summary>
	/// パーティクルエミッターを初期化します。
	/// </summary>
	/// <remarks>
	/// - DirectX共通インスタンス、テクスチャマネージャー、SRVマネージャーを取得して準備します。  
	/// - 指定されたカメラ情報を保持し、描画時に使用します。  
	/// - マテリアルリソースを生成し、初期色・照明フラグ・UV変換行列を設定します。  
	/// - GPU用インスタンシングバッファを生成し、最大インスタンス数分の座標変換行列や色を初期化します。  
	/// - SRVを確保して、構造化バッファに対応するビューを作成します。  
	/// - エミッター情報をJSONファイルから読み込み、ワールドトランスフォームを初期化します。  
	/// - 発生タイマー、生成数、発生フラグ、アクティブフラグを初期化します。  
	/// - モデルファイルが存在する場合はロードし、テクスチャを読み込み、モデルに設定します。  
	/// </remarks>
	void Initialize(const std::string& groupName, const std::string& fileName, Camera* camera);

	/// <summary>
	/// パーティクルエミッターの更新処理を行います。
	/// </summary>
	/// <remarks>
	/// - エミッター自身のワールドトランスフォームを更新します。  
	/// - アクティブ状態でない場合は更新処理をスキップします。  
	/// - パーティクル生成処理：  
	///   - 発生フラグが立っている場合、タイマーに基づきパーティクルを生成します。  
	///   - 最大生成数や無限生成フラグに応じて生成を制御します。  
	///   - 新規生成したパーティクルを管理リストに追加します。  
	/// - パーティクル更新処理：  
	///   - カメラのビュー・プロジェクション行列を取得し、ビルボード行列を計算します。  
	///   - 各パーティクルについて生存時間をチェックし、寿命を超えたパーティクルは削除します。  
	///   - 座標、回転、スケール、色を個別の更新モード（開始値、速度、イージング）に基づき計算します。  
	///   - 非ループ設定の場合はパーティクルの経過時間を進めます。  
	///   - ワールド行列を生成し、ビルボードやスケーリングを適用します。  
	///   - ワールド行列とビュー・プロジェクション行列を掛け合わせてWVP行列を作成し、インスタンシングデータに書き込みます。  
	/// - この更新により、描画時にGPU側で正しい変換や色が適用されるパーティクルデータが準備されます。  
	/// </remarks>
	void Update();

	/// <summary>
	/// パーティクルエミッターの描画処理を行います。
	/// </summary>
	/// <remarks>
	/// - アクティブ状態でない場合やインスタンス数が0の場合は描画をスキップします。  
	/// - 描画コマンドをラムダ式として作成し、レンダラーに登録します。  
	/// - 描画コマンド内で以下の処理を実施します：  
	///   - 共通描画設定を適用します。  
	///   - メッシュ、マテリアル、テクスチャのデータをGPUに送信します。  
	///   - インスタンシング用のSRVをバインドします。  
	///   - 登録されたパーティクルインスタンス数分、インスタンス描画コマンドを発行します。  
	/// </remarks>
	void Draw(LayerType layer);

	/// <summary>
	/// ImGuiの表示
	/// </summary>
	void ImGui();

	/// <summary>
	/// パーティクルの生成を開始します。
	/// </summary>
	/// <remarks>
	/// - 発生フラグを有効化し、パーティクル生成のカウンタとタイマーをリセットします。  
	/// </remarks>
	void Emit();

	/// <summary>
	/// エミッターの設定情報をJSONファイルに書き出します。
	/// </summary>
	/// <param name="groupName">エミッターが所属するグループ名。</param>
	/// <remarks>
	/// - エミッター名、モデル名、テクスチャ名、パーティクル生成条件などの各種パラメータをJSON形式で保存します。  
	/// - 位置・回転・拡縮・色のパラメータについては、開始値、終了値、ランダム範囲、速度、加速度なども含めて保存されます。  
	/// - ディレクトリが存在しない場合は自動で作成されます。  
	/// - 保存処理に失敗した場合はメッセージボックスで通知され、処理が停止します。  
	/// - JSONは見やすいようにインデント付きで出力されます。
	/// </remarks>
	void ExportEmitterData(const std::string& groupName);

	/// <summary>
	/// JSONファイルからエミッターの設定情報を読み込みます。
	/// </summary>
	/// <param name="groupName">エミッターが所属するグループ名。</param>
	/// <param name="fileName">読み込むエミッターファイル名</param>
	/// <remarks>
	/// - エミッター名、モデル名、テクスチャ名、パーティクル生成条件などをJSONから復元します。  
	/// - 位置・回転・拡縮・色の各パラメータについて、開始値、終了値、ランダム範囲、速度、加速度などを読み込みます。  
	/// - JSONファイルが存在しない場合や読み込みに失敗した場合は、メッセージボックスで通知され処理が停止します。  
	/// - 読み込んだデータは、内部メンバ変数に設定され、後続のパーティクル生成や描画処理で使用されます。
	/// </remarks>
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
	/// <remarks>
	/// - 位置、回転、拡縮、色に関するパラメータを、設定されている開始値とランダム範囲から初期化します。  
	/// - パーティクルのワールド座標はエミッターのワールド座標に基づき補正されます。  
	/// - transformメンバには、初期のスケール、回転、座標を設定します。  
	/// - パーティクルの寿命は、設定された寿命とランダム範囲からランダムに決定されます。  
	/// - currentTimeは0で初期化され、生成後の経過時間管理に使用されます。
	/// </remarks>
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
	/// <remarks>
	/// - STARTの場合は、パラメータを開始値に固定します。  
	/// - VELOCITYの場合は速度と加速度に基づき数値を更新します。  
	/// - EASINGの場合は経過時間に応じて補間を行い、指定された補間方法に従って値を計算します。  
	/// </remarks>
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
	/// <remarks>
	/// - ImGuiでは「##」以降の文字列が内部IDとして扱われるため、同じ表示名でも異なるIDを持たせたい場合に使用します。
	/// </remarks>
	std::string CreateLabelName(std::string labelName, const char* label);

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	/// === 他クラスからの借り物 === ///

	//DirectX基底
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

	//コライダーフラグ
	bool isCollision_;
};