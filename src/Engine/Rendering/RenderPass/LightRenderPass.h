#ifndef LIGHTRENDERPASS_H
#define LIGHTRENDERPASS_H

#include "Engine/Rendering/RenderPass/RenderPass.h"
#include "Engine/Object/Class/Mesh.h"

namespace Engine {
    class LightRenderPass : public RenderPass {
    private:
        Array<DynamicBuffer*> m_buffers;
        Array<Shader*> m_shaders;

    public:
        LightRenderPass(Context* context, SwapChain* swapchain);
        LightRenderPass(const LightRenderPass&) = default;
        virtual ~LightRenderPass();

        void Render(RenderPass* prev) override;

    private:
        void ClearOutputTarget();
        void ClearStencilTarget();

        void SetupBuffers();
        void SetupShaders();

        void UpdateCameraBuffer(Matrix4x4& world, Matrix4x4& view, Matrix4x4& proj);
        void UpdateAccumulationBuffer(Matrix4x4& world, Matrix4x4& view, Matrix4x4& proj);
        void UpdateLightBuffer(Vector4& value, Vector4& color);

        void StartRenderLightVolume(RenderPass* prev, Mesh* volume);
        void EndRenderLightVolume(RenderPass* prev, Mesh* volume);
    };
}

#endif // LIGHTRENDERPASS_H
