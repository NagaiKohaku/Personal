#pragma once

#include "3d/Camera/Camera.h"
#include "Math/WorldTransform.h"
#include "ParticleEmitter.h"

#include "list"

///=====================================================/// 
/// パーティクルエミッターのグループ管理クラス
///=====================================================///
class EmitterGroup {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="ptr">カメラ</param>
	void Initialize(Camera* ptr);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// ImGuiの表示
	/// </summary>
	void ImGui();

	/// <summary>
	/// エミッターの読み込み
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	void LoadEmitter(std::string fileName);

	/// <summary>
	/// エミッターの保存
	/// </summary>
	void SaveEmitter();

	/// <summary>
	/// エミッターの追加
	/// </summary>
	void AddEmitter();

	/// <summary>
	/// パーティクルを発生させる
	/// </summary>
	void Emit();

	/// <summary>
	/// パーティクルの生成を停止する
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