#ifndef MESHRENDERPASS_H
#define MESHRENDERPASS_H

#include "Engine/Rendering/RenderPass/RenderPass.h"

#include "Engine/Object/Class/Mesh.h"
#include "Engine/Object/Class/Material.h"
#include "Engine/Math/Matrix.h"

namespace Engine {
    class MeshRenderPass : public RenderPass {
    private:
        Array<DynamicBuffer*> m_buffers;
        Array<Shader*> m_shaders;

    public:
        MeshRenderPass(Context* context, SwapChain* swapchain);
        MeshRenderPass(const MeshRenderPass&) = default;
        virtual ~MeshRenderPass();

        void Render(RenderPass* prev) override;

    private:
        void AttachTargets();
        void ClearTargets();

        void SetupTargets();
        void SetupShaders();
        void SetupUniform();

        void UpdateBuffers(Submesh* meshDesc);
        void UpdateMaterial(Material* material);
        void UpdateUniform(Matrix4x4& world, Matrix4x4& view, Matrix4x4& proj);
    };
}

#endif // MESHRENDERPASS_H
