#include "Renderer.h"

#include "Base/DirectXCommon.h"
#include "Base/OffScreen.h"

///=====================================================/// 
/// Rendererのシングルトンインスタンスを取得
///=====================================================///
Renderer* Renderer::GetInstance() {
	static Renderer instance;
	return &instance;
}

///=====================================================/// 
/// レンダラーを初期化
///=====================================================///
void Renderer::Initialize() {

	//キューの初期化
	ClearQueue();

	//オフスクリーンキューの初期化
	ClearOffScreenQueue();
}

///=========================================================/// 
/// 登録された描画コマンドをレイヤー順に実行し、スワップチェーンに描画
///=========================================================///
void Renderer::SwapChainDraw() {

	//下位レイヤーから描画コマンドを実行
	for (const auto& [layer, func] : drawQueue_) {

		//レイヤーに入っている描画コマンドすべてを実行する
		for (int i = 0; i < drawQueue_[layer].size(); i++) {

			if (layer == LayerType::DEBUG) {

				if (isDebug()) {

					//描画コマンドを実行
					func[i]();
				}

				continue;
			}

			//描画コマンドを実行
			func[i]();
		}
	}

	//キューの初期化
	ClearQueue();
}

///=====================================================/// 
/// 登録されたオフスクリーン描画コマンドをレイヤー順に実行
///=====================================================///
void Renderer::OffScreenDraw() {

	//下位レイヤーから描画コマンドを実行
	for (const auto& [layer, func] : offScreenDrawQueue_) {

		//レイヤーに入っている描画コマンドすべてを実行する
		for (int i = 0; i < offScreenDrawQueue_[layer].size(); i++) {

			if (layer == LayerType::DEBUG) {

				if (isDebug()) {

					//描画コマンドを実行
					func[i]();
				}

				continue;
			}

			//描画コマンドを実行
			func[i]();
		}
	}

	//キューの初期化
	ClearOffScreenQueue();
}

///=====================================================/// 
/// 描画コマンドを指定したレイヤーと描画先に追加
///=====================================================///
void Renderer::AddDraw(LayerType layer, bool isOffScreen, std::function<void()> func) {

	if (isOffScreen) {

		//オフスクリーン描画コマンドを追加
		offScreenDrawQueue_[layer].push_back(func);
	} else {

		//描画コマンドを追加
		drawQueue_[layer].push_back(func);
	}

}

///=====================================================/// 
/// 通常描画キューに登録されている全ての描画コマンドをクリア
///=====================================================///
void Renderer::ClearQueue() {

	//レイヤーごとに初期化を実行
	for (auto& [layer, func] : drawQueue_) {

		//レイヤーに入っている描画コマンドをクリア
		func.clear();
	}
}

///=========================================================/// 
/// オフスクリーン描画キューに登録されている全ての描画コマンドをクリア
///=========================================================///
void Renderer::ClearOffScreenQueue() {

	//レイヤーごとに初期化を実行
	for (auto& [layer, func] : offScreenDrawQueue_) {

		//レイヤーに入っている描画コマンドをクリア
		func.clear();
	}
}

///=====================================================/// 
/// 現在のビルドがデバッグモードかどうかを判定
///=====================================================///
bool Renderer::isDebug() {

#ifdef _DEBUG
	return true;
#endif // _DEBUG

	return false;
}