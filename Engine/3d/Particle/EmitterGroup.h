#pragma once

#include "3d/Camera/Camera.h"
#include <Math/Transform/WorldTransform.h>
#include "ParticleEmitter.h"

#include "list"

/// <summary>
/// 複数のパーティクルエミッターをまとめて管理し、更新・描画を行うクラスです。
/// </summary>
class EmitterGroup {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// パーティクルエミッターグループの初期化を行います。
	/// </summary>
	void Initialize(Camera* ptr);

	/// <summary>
	/// パーティクルエミッターグループの更新処理を行います。
	/// </summary>
	void Update();

	/// <summary>
	/// パーティクルエミッターグループの描画処理を行います。
	/// </summary>
	void Draw();

	/// <summary>
	/// ImGuiの表示
	/// </summary>
	void ImGui();

	/// <summary>
	/// 指定されたJSONファイルからパーティクルエミッター情報を読み込み、グループに登録します。
	/// </summary>
	void LoadEmitter(std::string fileName);

	/// <summary>
	/// 現在のパーティクルエミッターグループの情報をJSONファイルに保存します。
	/// </summary>
	void SaveEmitter();

	/// <summary>
	/// デフォルトのパーティクルエミッターをグループに追加します。
	/// </summary>
	void AddEmitter();

	/// <summary>
	/// グループ内のすべてのパーティクルエミッターからパーティクルを発生させます。
	/// </summary>
	void Emit();

	/// <summary>
	/// グループ内のすべてのパーティクルエミッターの発生を停止します。
	/// </summary>
	void Stop();

	///-------------------------------------------/// 
	/// ゲッター・セッター
	///-------------------------------------------///
public:

	/// <summary>
	/// エミッターリストを取得
	/// </summary>
	/// <returns>エミッターリスト</returns>
	std::list<ParticleEmitter*> GetEmitterList();

	/// <summary>
	/// グループ名を取得
	/// </summary>
	/// <returns>グループ名</returns>
	std::string GetName() { return name_; }

	/// <summary>
	/// トランスフォームを取得
	/// </summary>
	/// <returns>トランスフォーム</returns>
	WorldTransform& GetWorldTransform() { return transform_; }

	/// <summary>
	/// テクスチャリストの設定
	/// </summary>
	/// <param name="list">テクスチャリスト</param>
	void SetTextureList(std::vector<std::string> list) { textureList_ = list; }

	/// <summary>
	/// トランスフォームの設定
	/// </summary>
	/// <param name="worldTransform">トランスフォーム</param>
	void SetWorldTransform(WorldTransform worldTransform) { transform_ = worldTransform; }

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//カメラ
	Camera* camera_;

	//エミッターのリスト
	std::list<std::unique_ptr<ParticleEmitter>> particleEmitters_;

	//テクスチャリスト
	std::vector<std::string> textureList_;

	//ワールドトランスフォーム
	WorldTransform transform_;

	//グループ名
	std::string name_;

	//ディレクトリパス
	std::string directoryPath_;
};