#pragma once

namespace MyEngine {

	class Camera;

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
		virtual void Initialize(Camera* cameraPtr);

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

		//カメラ
		Camera* camera_;

	};
}