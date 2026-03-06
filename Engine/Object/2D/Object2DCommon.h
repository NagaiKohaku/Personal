#pragma once

#include <Object/Base/ObjectCommonBase.h>

namespace MyEngine {

	class Object2DCommon : public ObjectCommonBase {

	public:

		Object2DCommon();

		~Object2DCommon();

	public:

		void Initialize(DirectXCommon* dxCommonPtr) override;

		void CommonDrawSetting() override;

		void CreateRootSignature() override;

		void CreateInputElement(
			std::vector<D3D12_INPUT_ELEMENT_DESC>& inputElements
		) override;

		void CreateShader(
			Microsoft::WRL::ComPtr<IDxcBlob>& vs,
			Microsoft::WRL::ComPtr<IDxcBlob>& ps
		) override;
	};
}