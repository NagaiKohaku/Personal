#pragma once

#include <memory>

namespace MyEngine {

	class WinApp;
	class DirectXCommon;
	class RTVManager;
	class DSVManager;
	class SRVManager;
	class OffScreen;
	class SpriteCommon;
	class ModelCommon;
	class Object2DCommon;
	class Object3DCommon;
	class DebugObjectCommon;
	class SkyBoxCommon;
	class ParticleCommon;
	class ImGuiManager;
	class Input;
	class Audio;
	class Renderer;

	class Framework {

	public:

		void Run();

		void PreDraw();

		void PostDraw();

		void BeginImGui();

		void EndImGui();

	public:

		Framework();

		virtual ~Framework();

		virtual void Initialize();

		virtual void Finalize();

		virtual void Update();

		virtual void Draw() = 0;

		virtual bool IsEndRequest() { return endRequest_; }

	protected:

		std::unique_ptr<WinApp> winApp_;

		std::unique_ptr<DirectXCommon> dxCommon_;

		std::unique_ptr<RTVManager> rtvManager_;

		std::unique_ptr<DSVManager> dsvManager_;

		std::unique_ptr<SRVManager> srvManager_;

		std::unique_ptr<OffScreen> offScreen_;

		std::unique_ptr<SpriteCommon> spriteCommon_;

		std::unique_ptr<ModelCommon> modelCommon_;

		std::unique_ptr<Object2DCommon> object2DCommon_;

		std::unique_ptr<Object3DCommon> object3DCommon_;

		std::unique_ptr<DebugObjectCommon> debugObjectCommon_;

		std::unique_ptr<SkyBoxCommon> skyBoxCommon_;

		std::unique_ptr<ParticleCommon> particleCommon_;

		std::unique_ptr<ImGuiManager> imGuiManager_;

		std::unique_ptr<Input> input_;

		std::unique_ptr<Audio> audio_;

		Renderer* renderer_;

	private:

		bool endRequest_ = false;
	};
}