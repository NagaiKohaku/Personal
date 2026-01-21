#pragma once
#include "d3dx12.h"
#include "d3d12.h"

#include "wrl.h"
#include "vector"

/// === 前方宣言 === ///

class DirectXCommon;

/// <summary>
/// パーティクル描画用の共通機能を提供するクラスです。
/// </summary>
class ParticleCommon {

	///-------------------------------------------/// 
	/// 列挙型
	///-------------------------------------------///
public:

	//ブレンドモードの種類
	enum class BlendTypeParticle {
		NORMAL,   //通常
		ADD,      //加算
		SUBTRUCT, //減算
		MULTILY,  //乗算
		SCREEN    //スクリーン
	};

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// ParticleCommonのシングルトンインスタンスを取得します。
	/// </summary>
	static ParticleCommon* GetInstance();

	/// <summary>
	/// ParticleCommonの初期化を行います。
	/// </summary>
	void Initialize();

	/// <summary>
	/// パーティクルの描画に必要な共通設定を行います。
	/// </summary>
	void CommonDrawSetting();

	///-------------------------------------------/// 
	/// クラス内処理関数
	///-------------------------------------------///
private:

	/// <summary>
	/// パーティクル描画用のルートシグネチャを作成します。
	/// </summary>
	void CreateRootSignature();

	/// <summary>
	/// パーティクル描画用のグラフィックパイプラインステートを作成します。
	/// </summary>
	void CreateGraphicsPipeline();

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//DirectX基底
	DirectXCommon* dxCommon_ = nullptr;

	//ブレンドモード
	BlendTypeParticle blendMode_;

	//ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;

	//グラフィックパイプラインステート
	std::vector<Microsoft::WRL::ComPtr<ID3D12PipelineState>> graphicsPipelineState_;

	///-------------------------------------------/// 
	/// セッター・ゲッター
	///-------------------------------------------///
public:

	/// <summary>
	/// DirectX基底を取得
	/// </summary>
	/// <returns>DirectX基底</returns>
	DirectXCommon* GetDxCommon() const { return dxCommon_; }

	/// <summary>
	/// ブレンドモードの設定
	/// </summary>
	/// <param name="blendType">ブレンドタイプ</param>
	void SetBlendMode(BlendTypeParticle blendType) { blendMode_ = blendType; }

};