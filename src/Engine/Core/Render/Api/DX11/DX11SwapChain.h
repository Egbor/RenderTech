#ifndef DX11SWAPCHAIN_H
#define DX11SWAPCHAIN_H

#include "Engine/Core/Render/Api/DX11/DX11Context.h"
#include "Engine/Core/Render/Base/Interface/ISwapChain.h"

namespace Engine {
    class DX11SwapChain : public ISwapChain {
    public:
        DX11SwapChain(DX11Context* context, Int32 width, Int32 height, HWND hWnd);
        virtual ~DX11SwapChain();

        Int32 GetWidth() const override;
        Int32 GetHeight() const override;

        void Swap() override;
        TargetResource* GetOutputTarget() const override;

    private:
        ComPtr<IDXGISwapChain> m_dxgiSwapChain;
        TargetResource* m_target;
    };
}

#endif // DX11SWAPCHAIN_H
