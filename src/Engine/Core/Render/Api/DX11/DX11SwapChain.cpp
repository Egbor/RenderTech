#include "Engine/Core/Render/Api/DX11/DX11SwapChain.h"
#include "Engine/Core/Render/Api/DX11/DX11Texture.h"
#include "Engine/Core/System/Exception/EngineException.h"

namespace Engine {
    DX11SwapChain::DX11SwapChain(DX11Context* dxContext, UInt32 width, UInt32 height, HWND hWnd) {
        HRESULT hr = 0;
        ComPtr<ID3D11Device> d3dDevice = dxContext->GetD3D11Device();

        ComPtr<IDXGIDevice> dxgiDevice;
        ComPtr<IDXGIAdapter> dxgiAdapter;
        ComPtr<IDXGIFactory> dxgiFactory;

        if (FAILED(hr = d3dDevice->QueryInterface(__uuidof(IDXGIDevice), &dxgiDevice))) {
            throw new EngineException("[DX11SwapChain] ID3DDevice::QueryInterface() failed.");
        }
        if (FAILED(hr = dxgiDevice->GetAdapter(&dxgiAdapter))) {
            throw new EngineException("[DX11SwapChain] IDXGIDevice::GetAdapter() failed.");
        }
        if (FAILED(hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), &dxgiFactory))) {
            throw new EngineException("[DX11SwapChain] IDXGIAdapter::GetParent() failed.");
        }

        DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
        ZeroMemory(&dxgiSwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
        dxgiSwapChainDesc.BufferCount = 1;
        dxgiSwapChainDesc.BufferDesc.Width = static_cast<UINT>(width);
        dxgiSwapChainDesc.BufferDesc.Height	= static_cast<UINT>(height);
        dxgiSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
        dxgiSwapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
        dxgiSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        dxgiSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        dxgiSwapChainDesc.OutputWindow = hWnd;
        dxgiSwapChainDesc.SampleDesc.Count = 1;
        dxgiSwapChainDesc.SampleDesc.Quality = 0;
        dxgiSwapChainDesc.Windowed = FALSE;

        if (FAILED(hr = dxgiFactory->CreateSwapChain(d3dDevice.Get(), &dxgiSwapChainDesc, &m_dxgiSwapChain))) {
            throw new EngineException("[DX11SwapChain] IDXGIFactory::CreateSwapChain() failed.");
        }

        m_dxBackBuffer = ClassType<DX11OutputTexture2D>::CreateObject(ObjectArgument::Dummy());
    }

    UInt32 DX11SwapChain::GetWidth() const {
        DXGI_SWAP_CHAIN_DESC desc;
        m_dxgiSwapChain->GetDesc(&desc);
        return static_cast<UInt32>(desc.BufferDesc.Width);
    }

    UInt32 DX11SwapChain::GetHeight() const {
        DXGI_SWAP_CHAIN_DESC desc;
        m_dxgiSwapChain->GetDesc(&desc);
        return static_cast<UInt32>(desc.BufferDesc.Height);
    }

    Texture2D* DX11SwapChain::GetBackBuffer() const {
        HRESULT hr = 0;

        ID3D11Texture2D* backBuffer;
        if (FAILED(hr = m_dxgiSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)))) {
            throw new EngineException("[DX11SwapChain] IDXGISwapChain::GetBuffer() failed.");
        }

        m_dxBackBuffer->Data().Initialize(backBuffer);
        return m_dxBackBuffer;
    }

    void DX11SwapChain::SwapBuffers() {
        m_dxgiSwapChain->Present(1, 0);
    }
}
