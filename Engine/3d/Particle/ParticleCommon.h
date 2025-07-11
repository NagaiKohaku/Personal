#pragma once
#include "d3dx12.h"
#include "d3d12.h"

#include "wrl.h"
#include "vector"

/// === 前方宣言 === ///

class DirectXCommon;

///=====================================================/// 
/// パーティクル基底クラス
///=====================================================///
class ParticleCommon {

	///-------------------------------------------/// 
	/// 列挙型
	///-------------------------------------------///
public:

	//ブレンドモードの種類
	enum BlendType {
		Normal,   //通常
		Add,      //加算
		Subtruct, //減算
		Multily,  //乗算
		Screen    //スクリーン
	};

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static ParticleCommon* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 描画前処理
	/// </summary>
	void CommonDrawSetting();

	///-------------------------------------------/// 
	/// クラス内処理関数
	///-------------------------------------------///
private:

	/// <summary>
	/// ルートシグネチャの生成
	/// </summary>
	void CreateRootSignature();

	/// <summary>
	/// グラフィックパイプラインの生成
	/// </summary>
	void CreateGraphicsPipeline();

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//DirectX基底
	DirectXCommon* dxCommon_ = nullptr;

	//ブレンドモード
	BlendType blendMode_;

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
	void SetBlendMode(BlendType blendType) { blendMode_ = blendType; }

};