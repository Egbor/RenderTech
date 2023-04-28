#ifndef SWAPCHAIN_H
#define SWAPCHAIN_H

#include "Engine/Object/Class/Texture.h"

namespace Engine {
    class SwapChain {
    public:
        SwapChain() = default;
        SwapChain(const SwapChain&) = default;
        virtual ~SwapChain() = default;

        virtual UInt32 GetWidth() const = 0;
        virtual UInt32 GetHeight() const = 0;

        virtual Texture2D* GetBackBuffer() const = 0;
        virtual void SwapBuffers() = 0;
    };
}

#endif // SWAPCHAIN_H
