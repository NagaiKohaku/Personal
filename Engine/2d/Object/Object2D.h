#pragma once

#include "DirectXTex.h"
#include "d3d12.h"

#include "Base/LayerType.h"

#include "2d/Object/Object2DCommon.h"
#include "2d/Sprite/Sprite.h"

#include <Math/Vector/Vector2.h>
#include <Math/Vector/Vector3.h>
#include <Math/Matrix/Matrix4x4.h>
#include <Math/Transform/WorldTransform.h>

#include "string"
#include "wrl.h"

/// <summary>
/// 2Dオブジェクトを表すクラス
/// </summary>
class Object2D {

	///-------------------------------------------/// 
	/// メンバ構造体
	///-------------------------------------------///
private:

	//座標変換行列データ
	struct TransformationMatrix {
		Matrix4x4 WVP;
		Matrix4x4 World;
		Matrix4x4 WorldInverseTranspose;
	};

	//座標変換データ
	struct Transform {
		Vector3 scale;
		Vector3 rotate;
		Vector3 translate;
	};

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// Object2Dの初期化を行います。
	/// </summary>
	void Initialize();

	/// <summary>
	/// Object2Dのスプライトおよび座標変換行列(WVP)を更新します。
	/// </summary>
	void Update();

	/// <summary>
	/// Object2Dを指定したレイヤーに描画登録します。
	/// </summary>
	/// <param name="layer"> 描画するレイヤー </param>
	void Draw(LayerType layer);

	/// <summary>
	/// ImGuiを使用してObject2Dのパラメータを表示します。
	/// </summary>
	void DisplayImGui();

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//2Dオブジェクト基底
	Object2DCommon* object2DCommon_;

	WorldTransform transform_;

	//座標変換行列リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> WVPResource_ = nullptr;

	//座標変換行列データ
	TransformationMatrix* WVPData_ = nullptr;

	//スプライト情報
	std::unique_ptr<Sprite> sprite_;

	//座標
	Vector2 translate_;

	//大きさ
	Vector2 size_;

	//角度
	float rotate_;

	///-------------------------------------------/// 
	/// ゲッター・セッター
	///-------------------------------------------///
public:

	WorldTransform& GetWorldTransform() { return transform_; }

	/// <summary>
	/// 座標を取得
	/// </summary>
	/// <returns>スプライトの座標</returns>
	Vector2 GetTranslate() const { return translate_; }

	/// <summary>
	/// サイズを取得
	/// </summary>
	/// <returns>スプライトのサイズ</returns>
	Vector2 GetSize() const { return size_; }

	/// <summary>
	/// 角度を取得
	/// </summary>
	/// <returns>スプライトの角度</returns>
	float GetRotate() const { return rotate_; }

	/// <summary>
	/// スプライト情報を取得
	/// </summary>
	/// <returns>スプライト情報</returns>
	Sprite* GetSprite() const { return sprite_.get(); }

	/// <summary>
	/// 座標を設定
	/// </summary>
	/// <param name="translate">スプライトの座標</param>
	void SetTranslate(const Vector2& translate) { translate_ = translate; }

	/// <summary>
	/// サイズを設定
	/// </summary>
	/// <param name="size">スプライトのサイズ</param>
	void SetSize(const Vector2& size) { size_ = size; }

	/// <summary>
	/// 角度を設定
	/// </summary>
	/// <param name="rotate">スプライトの角度</param>
	void SetRotate(const float& rotate) { rotate_ = rotate; }

	/// <summary>
	/// 名前で指定したスプライトをObject2Dに設定します。
	/// </summary>
	/// <param name="spriteName">割り当てるスプライトの登録名</param>
	void SetSprite(const std::string& spriteName);
};