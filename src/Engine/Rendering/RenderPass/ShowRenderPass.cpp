#include "Engine/Rendering/RenderPass/ShowRenderPass.h"

namespace Engine {
    ShowRenderPass::ShowRenderPass(Context* context, SwapChain* swapchain)
        : RenderPass(context, swapchain) {

    }

    void ShowRenderPass::Render(RenderPass* prev) {
        GetSwapChain()->SwapBuffers();
    }
}
