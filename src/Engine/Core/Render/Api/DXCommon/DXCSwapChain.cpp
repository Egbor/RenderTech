#include "Engine/Core/Render/Api/DXCommon/DXCSwapChain.h"

namespace Engine {
	DXCSwapChain::DXCSwapChain(const _Core* core, ComPtr<IDXGIFactory4> dxgiFactory, ComPtr<IUnknown> d3dDevice) {
        const IWindow* window = core->GetWindow();

        DXGI_SWAP_CHAIN_DESC1 dxgiSwapChainDesc;
        ZeroMemory(&dxgiSwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC1));
        dxgiSwapChainDesc.BufferCount = 2;
        dxgiSwapChainDesc.Width = static_cast<UINT>(window->GetWidth());
        dxgiSwapChainDesc.Height = static_cast<UINT>(window->GetHeight());
        dxgiSwapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        dxgiSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        dxgiSwapChainDesc.SampleDesc.Count = 1;
        dxgiSwapChainDesc.SampleDesc.Quality = 0;

        DXGI_SWAP_CHAIN_FULLSCREEN_DESC dxgiFullscreeDesc;
        dxgiFullscreeDesc.Windowed = TRUE;

        ComPtr<IDXGISwapChain1> dxgiSwapChain;
        if (FAILED(dxgiFactory->CreateSwapChainForHwnd(d3dDevice.Get(), reinterpret_cast<HWND>(window->WinId()), &dxgiSwapChainDesc, 
            &dxgiFullscreeDesc, nullptr, &dxgiSwapChain))) {
            _Core::InitiateFatalError(ENGINE_DEBUG_MESSAGE("IDXGIFactory::CreateSwapChain() failed"));
        }

        if (FAILED(dxgiSwapChain.As<IDXGISwapChain3>(&m_dxgiSwapChain))) {
            _Core::InitiateFatalError(ENGINE_DEBUG_MESSAGE("Casting from IDXGISwapChain to IDXGISwapChain3 failed"));
        }
	}

    Int32 DXCSwapChain::GetWidth() const {
        DXGI_SWAP_CHAIN_DESC desc;
        m_dxgiSwapChain->GetDesc(&desc);
        return static_cast<Int32>(desc.BufferDesc.Width);
    }

    Int32 DXCSwapChain::GetHeight() const {
        DXGI_SWAP_CHAIN_DESC desc;
        m_dxgiSwapChain->GetDesc(&desc);
        return static_cast<Int32>(desc.BufferDesc.Height);
    }

    TargetResource* DXCSwapChain::GetOutputTarget() const {
        return nullptr;
    }

    void DXCSwapChain::Swap() {
         m_dxgiSwapChain->Present(1, 0);
    }
}