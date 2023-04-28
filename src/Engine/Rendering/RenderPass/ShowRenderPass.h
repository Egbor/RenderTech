#ifndef SHOWRENDERPASS_H
#define SHOWRENDERPASS_H

#include "Engine/Rendering/RenderPass/RenderPass.h"

namespace Engine {
    class ShowRenderPass : public RenderPass {
    public:
        ShowRenderPass(Context* context, SwapChain* swapchain);
        ShowRenderPass(const ShowRenderPass&) = default;
        virtual ~ShowRenderPass() = default;

        void Render(RenderPass* prev) override;
    };
}

#endif // SHOWRENDERPASS_H
