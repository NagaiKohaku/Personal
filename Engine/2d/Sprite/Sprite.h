#pragma once

#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Matrix4x4.h"

#include "DirectXTex.h"
#include "d3d12.h"

#include "stdint.h"
#include "wrl.h"
#include "string"

/// === 前方宣言 === ///
class SpriteCommon;

///=====================================================/// 
/// スプライト
///=====================================================///
class Sprite {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="modelName">ファイルパス</param>
	void Initialize(const std::string& filePath);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// ImGuiの表示
	/// </summary>
	void DisplayImGui();

	/// <summary>
	/// テクスチャの変更
	/// </summary>
	/// <param name="modelName"></param>
	void ChangeTexture(const std::string& filePath);

	///-------------------------------------------/// 
	/// セッター・ゲッター
	///-------------------------------------------///
public:

	/// <summary>
	/// 座標のゲッター
	/// </summary>
	/// <returns>座標</returns>
	const Vector2& GetPosition() const { return position_; }

	/// <summary>
	/// 角度のゲッター
	/// </summary>
	/// <returns>角度</returns>
	float GetRotation() const { return rotation_; }

	/// <summary>
	/// サイズのゲッター
	/// </summary>
	/// <returns>サイズ</returns>
	const Vector2& GetSize() const { return size_; }

	/// <summary>
	/// 色のゲッター
	/// </summary>
	/// <returns>色</returns>
	const Vector4& GetColor() const { return materialData_->color; }

	/// <summary>
	/// アンカーポイントのゲッター
	/// </summary>
	/// <returns>アンカーポイント</returns>
	const Vector2& GetAnchorPoint() const { return anchorPoint_; }

	/// <summary>
	/// X軸の反転フラグのゲッター
	/// </summary>
	/// <returns>X軸の反転フラグ</returns>
	bool GetIsFlipX() const { return isFlipX_; }

	/// <summary>
	/// Y軸の反転フラグのゲッター
	/// </summary>
	/// <returns>Y軸の反転フラグ</returns>
	bool GetIsFlipY() const { return isFlipY_; }

	/// <summary>
	/// テクスチャの左上座標のゲッター
	/// </summary>
	/// <returns>テクスチャの左上座標</returns>
	const Vector2& GetTextureLeftTop() const { return textureLeftTop_; }

	/// <summary>
	/// テクスチャのサイズのゲッター
	/// </summary>
	/// <returns>テクスチャのサイズ</returns>
	const Vector2& GetTextureSize() const { return textureSize_; }

	/// <summary>
	/// 座標のセッター
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(const Vector2& position) { position_ = position; }

	/// <summary>
	/// 角度のセッター
	/// </summary>
	/// <param name="rotation">角度</param>
	void SetRotation(float rotation) { rotation_ = rotation; }

	/// <summary>
	/// サイズのゲッター
	/// </summary>
	/// <param name="size">サイズ</param>
	void SetSize(const Vector2& size) { size_ = size; }

	/// <summary>
	/// 色のセッター
	/// </summary>
	/// <param name="color">色</param>
	void SetColor(const Vector4& color) { materialData_->color = color; }

	/// <summary>
	/// アンカーポイントのセッター
	/// </summary>
	/// <param name="anchorPoint">アンカーポイント</param>
	void SetAnchorPoint(const Vector2& anchorPoint) { anchorPoint_ = anchorPoint; }

	/// <summary>
	/// X軸の反転フラグのセッター
	/// </summary>
	/// <param name="isFlipX">X軸の反転フラグ</param>
	void SetIsFlipX(const bool isFlipX) { isFlipX_ = isFlipX; }

	/// <summary>
	/// Y軸の反転フラグのセッター
	/// </summary>
	/// <param name="isFlipX">Y軸の反転フラグ</param>
	void SetIsFlipY(const bool isFlipY) { isFlipY_ = isFlipY; }

	/// <summary>
	/// テクスチャの左上座標のセッター
	/// </summary>
	/// <param name="textureLeftTop">テクスチャの左上座標</param>
	void SetTextureLeftTop(const Vector2& textureLeftTop) { textureLeftTop_ = textureLeftTop; }

	/// <summary>
	/// テクスチャのサイズのセッター
	/// </summary>
	/// <param name="textureSize">テクスチャのサイズ</param>
	void SetTextureSize(const Vector2& textureSize) { textureSize_ = textureSize; }

	///-------------------------------------------/// 
	/// クラス内処理関数
	///-------------------------------------------///
private:

	/// <summary>
	/// スプライトのサイズをテクスチャのサイズに合わせる
	/// </summary>
	void AdjustTextureSize();

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

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//スプライト基底
	SpriteCommon* spriteCommon_ = nullptr;

	/// === バッファリソース === ///

	//頂点リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_ = nullptr;

	//頂点番号リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> IndexResource_ = nullptr;

	//マテリアルリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_ = nullptr;

	/// === バッファリソース内のデータを指すポインタ === ///

	//頂点データ
	VertexData* vertexData_ = nullptr;

	//頂点番号データ
	uint32_t* indexData_ = nullptr;

	//マテリアルデータ
	Material* materialData_ = nullptr;

	/// === バッファビュー === ///

	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

	//頂点番号バッファビュー
	D3D12_INDEX_BUFFER_VIEW indexBufferView_;

	//テクスチャパス
	std::string texturePath_;

	//座標
	Vector2 position_{ 0.0f,0.0f };

	//角度
	float rotation_ = 0.0f;

	//サイズ
	Vector2 size_ = { 640.0f,360.0f };

	//アンカーポイント
	Vector2 anchorPoint_ = { 0.0f,0.0f };

	//X軸の反転フラグ
	bool isFlipX_ = false;

	//Y軸の反転フラグ
	bool isFlipY_ = false;

	//テクスチャの左上座標
	Vector2 textureLeftTop_ = { 0.0f,0.0f };

	//テクスチャのサイズ
	Vector2 textureSize_ = { 100.0f,100.0f };

};