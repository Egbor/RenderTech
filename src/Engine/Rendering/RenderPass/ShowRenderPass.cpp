#include "Engine/Rendering/RenderPass/ShowRenderPass.h"

namespace Engine {
    ShowRenderPass::ShowRenderPass(Context* context, Texture2D* target)
        : ContentRenderPass(context, target) {

    }

    void ShowRenderPass::Render(RenderPass* prev) {
        //GetSwapChain()->SwapBuffers();
    }
}
