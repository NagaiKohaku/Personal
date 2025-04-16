#include "Renderer.h"

#include "Base/DirectXCommon.h"

///=====================================================/// 
/// 静的インスタンスの取得
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
/// 描画
///=====================================================///
void Renderer::Draw() {

	//下位レイヤーから描画コマンドを実行
	for (const auto& [layer, func] : drawQueue_) {

		//レイヤーに入っている描画コマンドすべてを実行する
		for (int i = 0; i < drawQueue_[layer].size(); i++) {

			//描画コマンドを実行
			func[i]();
		}

		//深度情報をクリア
		DirectXCommon::GetInstance()->ClearDepthBuffer();
	}

	//キューの初期化
	ClearQueue();
}

void Renderer::OffScreenDraw() {

	//下位レイヤーから描画コマンドを実行
	for (const auto& [layer, func] : offScreenDrawQueue_) {

		//レイヤーに入っている描画コマンドすべてを実行する
		for (int i = 0; i < offScreenDrawQueue_[layer].size(); i++) {

			//描画コマンドを実行
			func[i]();
		}

		//深度情報をクリア
		DirectXCommon::GetInstance()->ClearDepthBuffer();
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

void Renderer::ClearOffScreenQueue() {

	//レイヤーごとに初期化を実行
	for (auto& [layer, func] : offScreenDrawQueue_) {
		//レイヤーに入っている描画コマンドをクリア
		func.clear();
	}
}