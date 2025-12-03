#pragma once
#include "Scene/BaseScene.h"

#include "Base/Audio.h"
#include "2d/Object/Object2D.h"
#include "3d/Camera/Camera.h"
#include "3d/Object/Object3D.h"
#include "3d/Object/DebugLine.h"
#include "3d/Particle/ParticleEmitter.h"
#include "3d/Particle/EmitterGroup.h"
#include "3d/Object/DebugLine.h"

#include "memory"

class ParticleEditorScene : public BaseScene {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// パーティクルエディタ用シーンの初期化を行います。
	/// </summary>
	/// <remarks>
	/// - カメラを生成・初期化し、デバッグカメラとして設定。カメラの位置・回転を設定し、デフォルトカメラとして登録します。  
	/// - Particle用スプライトテクスチャをResourceディレクトリから取得し、TextureManagerでロードします。  
	/// - エミッターグループ用のディレクトリからグループ名を取得し、デフォルトグループを生成します。  
	/// - 床オブジェクト（ライン）を生成し、X方向・Z方向のグリッドラインを描画。中央ラインは赤・緑で色分けします。  
	/// - lines_に格納することで描画・更新時に利用可能にします。  
	/// </remarks>
	void Initialize() override;

	/// <summary>
	/// パーティクルエディタシーンの毎フレーム更新処理を行います。
	/// </summary>
	/// <remarks>
	/// - カメラをデバッグモードで更新します。  
	/// - 登録されているすべてのエミッターグループを更新し、パーティクルの生成・移動・ライフサイクルを進めます。  
	/// - 床用のデバッグラインを更新し、描画や座標情報を最新化します。  
	/// </remarks>
	void Update() override;

	/// <summary>
	/// パーティクルエディタシーンの描画処理を行います。
	/// </summary>
	/// <remarks>
	/// - 登録されているすべてのエミッターグループを描画します。  
	///   各グループは内部のパーティクルを描画します。  
	/// - 床用のデバッグラインを描画し、シーン内の座標やガイドラインとして表示します。  
	/// - 描画順はパーティクル → デバッグラインの順で行います。  
	/// </remarks>
	void Draw() override;

	/// <summary>
	/// パーティクルエディタシーンのImGui表示処理を行います。
	/// </summary>
	void ImGui() override;

	///-------------------------------------------/// 
	/// クラス内処理関数
	///-------------------------------------------///
private:

	/// <summary>
	/// デフォルトのパーティクルエミッターグループを生成します。
	/// </summary>
	/// <remarks>
	/// - 新しいEmitterGroupを生成し、カメラ参照を設定します。  
	/// - 利用可能なテクスチャリストを渡します。  
	/// - "defaultGroup" のエミッター設定を読み込みます。  
	/// - 作成したグループをemitterGroups_に追加します。  
	/// </remarks>
	void CreateGroup();

	/// <summary>
	/// 指定した名前のパーティクルエミッターグループを読み込みます。
	/// </summary>
	/// <param name="groupName">読み込むエミッターグループの名前</param>
	/// <remarks>
	/// - 新しいEmitterGroupを生成し、カメラ参照を設定します。  
	/// - 利用可能なテクスチャリストを渡します。  
	/// - 指定されたgroupNameのエミッター設定を読み込みます。  
	/// - 作成したグループをemitterGroups_に追加します。  
	/// </remarks>
	void LoadGroup(const std::string& groupName = "defaultGroup");

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//カメラ
	std::unique_ptr<Camera> camera_;

	std::list<std::unique_ptr<EmitterGroup>> emitterGroups_;

	std::list<std::string> emitterGroupNames_;

	std::vector<std::unique_ptr<DebugLine>> lines_;

	std::vector<std::string> textureList_;

	float lineDivide_;

};