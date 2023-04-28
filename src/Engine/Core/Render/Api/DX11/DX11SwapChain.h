#ifndef DX11SWAPCHAIN_H
#define DX11SWAPCHAIN_H

#include "Engine/Core/Render/Api/SwapChain.h"
#include "Engine/Core/Render/Api/DX11/DX11Texture.h"

namespace Engine {
    class DX11SwapChain : public SwapChain {
    private:
        DX11Texture2D* m_dxBackBuffer;
        ComPtr<IDXGISwapChain> m_dxgiSwapChain;

    public:
        DX11SwapChain(DX11Context* dxContext, UInt32 width, UInt32 height, HWND hWnd);
        DX11SwapChain(const DX11SwapChain&) = default;
        virtual ~DX11SwapChain() = default;

        UInt32 GetWidth() const override;
        UInt32 GetHeight() const override;

//        Texture2D* CreateBackBuffer() const override;
        Texture2D* GetBackBuffer() const override;
        void SwapBuffers() override;
    };
}

#endif // DX11SWAPCHAIN_H
