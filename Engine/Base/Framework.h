#pragma once

#include <Base/WinApp.h>
#include <Base/DirectXCommon.h>
#include <Base/View/RTVManager.h>
#include <Base/View/DSVManager.h>
#include <Base/View/SRVManager.h>
#include <Base/OffScreen.h>
#include <2d/Sprite/SpriteCommon.h>
#include <3d/Model/ModelCommon.h>
#include <2d/Object/Object2DCommon.h>
#include <3d/Object/Object3DCommon.h>
#include <3d/Object/DebugObjectCommon.h>
#include <3d/Object/SkyBoxCommon.h>
#include <3d/Particle/ParticleCommon.h>
#include <Base/ImGuiManager.h>
#include <Base/Renderer.h>
#include <Base/Input.h>
#include <Base/Audio.h>

namespace MyEngine {

	class Framework {

	public:

		void Run();

		void PreDraw();

		void PostDraw();

		void BeginImGui();

		void EndImGui();

	public:

		virtual ~Framework() = default;

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

		Renderer* renderer_;

		Audio* audio_;

		Input* input_;

	private:

		bool endRequest_ = false;
	};
}