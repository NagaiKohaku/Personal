#pragma once

#include "Base/LayerType.h"

#include <Object/Component/Geometry/Model/Model.h>
#include <Object/Component/Geometry/Sprite/Sprite.h>

#include "functional"
#include "map"
#include "vector"
#include "string"

namespace MyEngine {

	/// <summary>
	/// 描画処理を統括し、登録された描画コマンドをレイヤー順に実行するクラスです。
	/// </summary>
	class Renderer {

	public:

		Renderer();

		~Renderer();

		///-------------------------------------------/// 
		/// メンバ関数
		///-------------------------------------------///
	public:

		/// <summary>
		/// レンダラーを初期化します。
		/// </summary>
		void Initialize();

		/// <summary>
		/// 登録された描画コマンドをレイヤー順に実行し、スワップチェーンに描画します。
		/// </summary>
		void SwapChainDraw();

		/// <summary>
		/// 登録されたオフスクリーン描画コマンドをレイヤー順に実行します。
		/// </summary>
		void OffScreenDraw();

		/// <summary>
		/// 描画コマンドを指定したレイヤーと描画先に追加します。
		/// </summary>
		void AddDraw(const std::string& modelName, std::function<void()> func);

		///-------------------------------------------/// 
		/// クラス内関数
		///-------------------------------------------///
	private:

		/// <summary>
		/// 通常描画キューに登録されている全ての描画コマンドをクリアします。
		/// </summary>
		void ClearQueue();

		/// <summary>
		/// オフスクリーン描画キューに登録されている全ての描画コマンドをクリアします。
		/// </summary>
		void ClearOffScreenQueue();

		/// <summary>
		/// 現在のビルドがデバッグモードかどうかを判定します。
		/// </summary>
		bool isDebug();

		///-------------------------------------------/// 
		/// メンバ変数
		///-------------------------------------------///
	private:

		//描画コマンドキュー
		std::map<std::string, std::vector<std::function<void()>>> drawQueue_;

		//OffScreen用の描画コマンドキュー
		std::map<std::string, std::vector<std::function<void()>>> offScreenDrawQueue_;

		std::vector<Model*> drawModels_;

		std::vector<Sprite*> drawSprites_;
	};
}