#ifndef DXCSWAPCHAIN_H
#define DXCSWAPCHAIN_H

#include "Engine/Core/Core.h"
#include "Engine/Core/Render/Api/DXCommon/DXCDef.h"
#include "Engine/Core/Render/Base/Interface/ISwapChain.h"

namespace Engine {
	class DXCSwapChain : public ISwapChain {
	public:
		DXCSwapChain(const _Core* core, ComPtr<IDXGIFactory4> dxgiFactory, ComPtr<IUnknown> d3dDevice);

		Int32 GetWidth() const override;
		Int32 GetHeight() const override;

		void Swap() override;
		TargetResource* GetOutputTarget() const override;

	private:
		ComPtr<IDXGISwapChain3> m_dxgiSwapChain;
	};
}

#endif // !DXCSWAPCHAIN_H
