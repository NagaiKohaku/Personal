#pragma once

#include <Scene/EngineContext.h>

namespace MyEngine {

	///=====================================================/// 
	/// ベースシーン
	///=====================================================///
	class BaseScene {

		///-------------------------------------------/// 
		/// メンバ関数
		///-------------------------------------------///
	public:

		/// <summary>
		/// 仮想デストラクタ
		/// </summary>
		virtual ~BaseScene() = default;

		/// <summary>
		/// 初期化
		/// </summary>
		virtual void Initialize(EngineContext context);

		/// <summary>
		/// 終了処理
		/// </summary>
		virtual void Finalize();

		/// <summary>
		/// 更新
		/// </summary>
		virtual void Update();

		/// <summary>
		/// 描画
		/// </summary>
		virtual void Draw();

		/// <summary>
		/// デバッグ
		/// </summary>
		virtual void ImGui();

	protected:

		//エンジン総合参照
		EngineContext context_;
	};
}