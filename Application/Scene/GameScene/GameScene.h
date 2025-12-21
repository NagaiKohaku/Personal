#pragma once
#include "Scene/BaseScene.h"

#include "Base/Audio.h"
#include "2d/Object/Object2D.h"
#include "3d/Camera/Camera.h"
#include "3d/Object/Object3D.h"
#include "3d/Object/DebugLine.h"
#include "3d/Object/SkyBox.h"
#include "3d/Particle/ParticleEmitter.h"
#include "3d/Particle/EmitterGroup.h"

#include <Scene/GameScene/GameScene3DObject.h>
#include <Scene/GameScene/GameScene2DObject.h>
#include <Scene/GameScene/Event/StartEvent.h>
#include <Scene/GameScene/Event/ClearEvent.h>
#include <Scene/GameScene/Event/GameOverEvent.h>

#include "memory"
#include "vector"
#include "list"

/// <summary>
/// ゲームシーンを表すクラスです。
/// </summary>
/// <remarks>
/// - BaseSceneを継承し、ゲームプレイに必要なオブジェクトの管理、更新、描画、アニメーション演出を行います。  
/// - カメラ（通常カメラ・追従カメラ）、プレイヤー、エネミー、弾、グラウンド、UIスプライト、エフェクトなどのオブジェクトを管理します。  
/// - ゲーム開始時・クリア時のアニメーションや、ゲームオーバー演出の制御も行います。  
/// - Update()やDraw()内で各オブジェクトの更新・描画を順序よく行い、シーン遷移やフェード制御も含めています。  
/// - AnimPoint構造体を用いたキーフレーム方式で、プレイヤーとカメラのアニメーションを補間します。  
/// </remarks>
class GameScene : public BaseScene {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// ゲームシーンの初期化を行います。
	/// </summary>
	/// <remarks>
	/// - カメラを生成・初期化し、デバッグカメラを無効化した後、シーン全体のカメラ制御に設定します。  
	/// - エネミーマネージャー、バレットマネージャー、グラウンドマネージャーを生成・初期化します。  
	/// - プレイヤーを生成し、カメラとバレットマネージャーを渡して初期化します。初期状態では移動不可に設定します。  
	/// - 追尾カメラを生成・初期化し、プレイヤーに追従させます。初期状態では無効化します。  
	/// - ゲームオーバーおよびゲームクリア関連の2Dスプライトを生成し、位置・アンカーポイント・カラーを設定します。  
	/// - 衝撃波エミッターを左右に生成・初期化し、カメラに紐づけて読み込みます。  
	/// - 各種タイマーやフラグを初期化し、シーン開始時のアニメーション用キーフレームを設定します。  
	/// - Fade制御にカメラとプレイヤーをセットし、フェードインを開始します。  
	/// </remarks>
	void Initialize() override;

	/// <summary>
	/// ゲームシーンの終了処理を行います。
	/// </summary>
	/// <remarks>
	/// - ObjectManagerに登録されている全てのオブジェクトをクリアします。  
	/// - 演出系（フェード・カメラシェイク）の参照をリセットし、外部オブジェクトへの依存を解除します。  
	/// - 使用している音声データやオーディオシステムを解放してリソースを整理します。  
	/// </remarks>
	void Finalize() override;

	/// <summary>
	/// ゲームシーンの毎フレーム更新処理を行います。
	/// </summary>
	/// <remarks>
	/// - フェード演出の状態を確認し、FADE_INが終了していれば状態をリセットします。  
	/// - スタートおよびクリア時のアニメーションを更新します。  
	/// - 追尾カメラおよびメインカメラを更新します。  
	/// - ゲーム進行中であれば、プレイヤー・エネミー・弾・グラウンドの各オブジェクトを更新します。  
	/// - ゲームクリア時にはTransformのみ更新して描画状態を維持します。  
	/// - UIの矢印アニメーションをタイマーで制御し、Lerp/EaseOutでスムーズに移動させます。  
	/// - ゲームオーバー・ゲームクリアスプライトの更新と表示制御を行います。  
	/// - 衝撃波エミッターを更新します。  
	/// - プレイヤーが死亡した場合、ゲームオーバー演出を開始します。  
	/// - 一定条件を満たした場合、ゲームクリア演出を開始し、フェードアウトによるシーン遷移を行います。  
	/// </remarks>
	void Update() override;

	/// <summary>
	/// ゲームシーンの描画処理を行います。
	/// </summary>
	/// <remarks>
	/// - プレイヤー、エネミー、弾、グラウンドをそれぞれ描画します。  
	/// - ゲームオーバー・ゲームクリア時のスプライトをUIレイヤーとして描画します。  
	/// - ゲームクリア演出中でなければ、ヘルプスプライトを描画します。  
	/// - 衝撃波エミッターを左右それぞれ描画します。  
	/// - 描画順は基本的にゲームオブジェクト → エフェクト → UIスプライトの順で行います。  
	/// </remarks>
	void Draw() override;

	/// <summary>
	/// デバッグ
	/// </summary>
	void ImGui() override;

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	std::unique_ptr<GameScene3DObject> object3D_;

	std::unique_ptr<GameScene2DObject> object2D_;

	std::unique_ptr<StartEvent> startEvent_;

	std::unique_ptr<ClearEvent> clearEvent_;

	std::unique_ptr<GameOverEvent> gameOverEvent_;
};