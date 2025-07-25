#include "Renderer.h"

#include "Base/DirectXCommon.h"
#include "Base/OffScreen.h"

///=====================================================/// 
/// シングルトンインスタンスの取得
///=====================================================///
Renderer* Renderer::GetInstance() {
	static Renderer instance;
	return &instance;
}

///=====================================================/// 
/// 初期化
///=====================================================///
void Renderer::Initialize() {

	//キューの初期化
	ClearQueue();
}

///=====================================================/// 
/// スワップチェーン描画
///=====================================================///
void Renderer::SwapChainDraw() {

	//下位レイヤーから描画コマンドを実行
	for (const auto& [layer, func] : drawQueue_) {

		//レイヤーに入っている描画コマンドすべてを実行する
		for (int i = 0; i < drawQueue_[layer].size(); i++) {

			if (layer == LayerType::Debug) {

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
/// オフスクリーン描画
///=====================================================///
void Renderer::OffScreenDraw() {

	//下位レイヤーから描画コマンドを実行
	for (const auto& [layer, func] : offScreenDrawQueue_) {

		//レイヤーに入っている描画コマンドすべてを実行する
		for (int i = 0; i < offScreenDrawQueue_[layer].size(); i++) {

			if (layer == LayerType::Debug) {

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
/// 描画コマンドの追加
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
/// 描画コマンドキューの初期化
///=====================================================///
void Renderer::ClearQueue() {

	//レイヤーごとに初期化を実行
	for (auto& [layer, func] : drawQueue_) {

		//レイヤーに入っている描画コマンドをクリア
		func.clear();
	}
}

///=====================================================/// 
/// オフスクリーンキューの初期化
///=====================================================///
void Renderer::ClearOffScreenQueue() {

	//レイヤーごとに初期化を実行
	for (auto& [layer, func] : offScreenDrawQueue_) {

		//レイヤーに入っている描画コマンドをクリア
		func.clear();
	}
}

bool Renderer::isDebug() {

#ifdef _DEBUG
	return true;
#endif // _DEBUG

	return false;
}