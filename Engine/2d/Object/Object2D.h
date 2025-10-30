#pragma once

#include "DirectXTex.h"
#include "d3d12.h"

#include "Base/LayerType.h"

#include "2d/Object/Object2DCommon.h"
#include "2d/Sprite/Sprite.h"

#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Matrix4x4.h"

#include "string"
#include "wrl.h"

/// <summary>
/// 2Dオブジェクトを表すクラス
/// </summary>
/// <remarks>
/// - スプライトの管理
/// - 座標変換行列の更新
/// - 描画登録をRendererに対して行う
/// </remarks>
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
	/// <remarks>
	/// - Object2DCommonのインスタンスを取得
	/// - 座標変換用リソース(WVPResource)の作成とマッピング
	/// - WVPデータを単位行列で初期化
	/// - 座標(translate)、サイズ(size)、回転角(rotate)を初期値に設定
	/// </remarks>
	void Initialize();

	/// <summary>
	/// Object2Dのスプライトおよび座標変換行列(WVP)を更新します。
	/// </summary>
	/// <remarks>
	/// - 登録されているスプライト(sprite_)のUpdate()を呼び出す
	/// - 座標変換行列データの更新
	/// </remarks>
	void Update();

	/// <summary>
	/// Object2Dを指定したレイヤーに描画登録します。
	/// </summary>
	/// <param name="layer"> 描画するレイヤー </param>
	/// <remarks>
	/// - 描画用の関数をラムダ式で作成
	///   - Object2DCommonの共通描画設定を適用
	///   - 座標変換行列データの設定
	///   - 割り当てられたスプライトを描画
	/// - 作成した描画関数をRendererにAddDrawで登録
	///
	/// layerには描画順序を示すLayerTypeを指定します。
	/// 描画登録のみを行い、実際の描画はRendererによってフレーム末に行われます。
	/// </remarks>
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
	/// <remarks>
	/// spriteNameで指定したスプライトをSpriteManagerから検索し、
	/// 見つかった場合はObject2Dのsprite_に設定します。
	/// 設定後、Object2Dのサイズ(size_)をスプライトのサイズに合わせます。
	///
	/// spriteNameに対応するスプライトが存在しない場合、
	/// sprite_はnullptrになりますので注意してください。
	/// </remarks>
	void SetSprite(const std::string& spriteName);
};