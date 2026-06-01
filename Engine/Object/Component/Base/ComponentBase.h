#pragma once

namespace MyEngine {

	class ComponentBase {

	public:

		virtual void Initialize() = 0;

		virtual void Update() = 0;

		virtual void SendDataForGPU() = 0;

		virtual void Debug() = 0;
	};
}