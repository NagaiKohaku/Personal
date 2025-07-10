#pragma once

#include "DirectXTex.h"
#include "d3d12.h"

#include "Base/LayerType.h"

#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Matrix4x4.h"

#include "string"
#include "vector"
#include "wrl.h"

/// === 前方宣言 === ///

class Object2DCommon;

class Sprite;

///=====================================================/// 
/// 2Dオブジェクトクラス
///=====================================================///
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
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(LayerType layer);

	/// <summary>
	/// ImGuiの描画
	/// </summary>
	void DisplayImGui();

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//2Dオブジェクト基底
	Object2DCommon* object2DCommon_;

	//座標変換行列リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> WVPResource_ = nullptr;

	//座標変換行列データ
	TransformationMatrix* WVPData_ = nullptr;

	//座標
	Vector2 translate_;

	//大きさ
	Vector2 size_;

	//角度
	float rotate_;

	//スプライト情報
	Sprite* sprite_;

	///-------------------------------------------/// 
	/// ゲッター・セッター
	///-------------------------------------------///
public:

	/// <summary>
	/// 座標を取得
	/// </summary>
	/// <returns>座標</returns>
	Vector2 GetTranslate() const { return translate_; }

	/// <summary>
	/// サイズを取得
	/// </summary>
	/// <returns>サイズ</returns>
	Vector2 GetSize() const { return size_; }

	/// <summary>
	/// 角度を取得
	/// </summary>
	/// <returns>角度</returns>
	float GetRotate() const { return rotate_; }

	/// <summary>
	/// スプライトを取得
	/// </summary>
	/// <returns>スプライト</returns>
	Sprite* GetSprite() const { return sprite_; }

	/// <summary>
	/// 座標を設定
	/// </summary>
	/// <param name="translate">座標</param>
	void SetTranslate(const Vector2& translate) { translate_ = translate; }

	/// <summary>
	/// サイズを設定
	/// </summary>
	/// <param name="size">サイズ</param>
	void SetSize(const Vector2& size) { size_ = size; }

	/// <summary>
	/// 角度を設定
	/// </summary>
	/// <param name="rotate">角度</param>
	void SetRotate(const float& rotate) { rotate_ = rotate; }

	/// <summary>
	/// スプライトのセッター
	/// </summary>
	/// <param name="modelName">スプライト名</param>
	void SetSprite(const std::string& spriteName);
};