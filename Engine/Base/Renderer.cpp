#include "Renderer.h"

#include "Base/DirectXCommon.h"
#include "Base/OffScreen.h"
#include "Object/3D/Model/ModelManager.h"

namespace MyEngine {

	Renderer::Renderer() = default;

	Renderer::~Renderer() = default;

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

				//描画コマンドを実行
				func[i]();
			}
		}

		//キューの初期化
		ClearOffScreenQueue();
	}

	void Renderer::AddDraw(const std::string& modelName, std::function<void()> func) {

		if (ModelManager::GetInstance()->GetModel(modelName)) {

			offScreenDrawQueue_[modelName].push_back(func);
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
}