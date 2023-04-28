#ifndef SKYBOXRENDERPASS_H
#define SKYBOXRENDERPASS_H

#include "Engine/Rendering/RenderPass/RenderPass.h"

namespace Engine {
    class SkyboxRenderPass : public RenderPass {
    private:
        Array<DynamicBuffer*> m_buffers;
        Array<Shader*> m_shaders;

    public:
        SkyboxRenderPass(Context* context, SwapChain* swapchain);
        SkyboxRenderPass(const SkyboxRenderPass&) = default;
        virtual ~SkyboxRenderPass();

        void Render(RenderPass* prev) override;

        void SetupShaders();
        void SetupBuffers();
        void SetupTextures();

        void UpdateWVPBuffer(Matrix4x4& world, Matrix4x4& view, Matrix4x4& proj);
    };
}

#endif // SKYBOXRENDERPASS_H
