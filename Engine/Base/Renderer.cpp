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

///=====================================================/// 
/// 描画コマンドの追加
///=====================================================///
void Renderer::AddDraw(LayerType layer, std::function<void()> func) {

	//指定されたレイヤーに描画コマンドを追加
	drawQueue_[layer].push_back(func);
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