#ifndef SHOWRENDERPASS_H
#define SHOWRENDERPASS_H

#include "Engine/Rendering/RenderPass/ContentRenderPass.h"

namespace Engine {
    class ShowRenderPass : public ContentRenderPass {
    public:
        ShowRenderPass(Context* context, Texture2D* target);
        ShowRenderPass(const ShowRenderPass&) = default;
        virtual ~ShowRenderPass() = default;

        void Render(RenderPass* prev) override;
    };
}

#endif // SHOWRENDERPASS_H
