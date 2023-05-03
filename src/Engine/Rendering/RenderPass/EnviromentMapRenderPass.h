#ifndef CUBEMAPRENDERPASS_H
#define CUBEMAPRENDERPASS_H

#include "Engine/Rendering/RenderPass/ContentRenderPass.h"
#include "Engine/Object/Class/Mesh.h"

namespace Engine {
    class EnviromentMapRenderPass : public ContentRenderPass {
    private:
        Array<DynamicBuffer*> m_buffers;
        Array<Shader*> m_shaders;

        Mesh* m_cubeMesh;
        Array<Matrix4x4> m_cameraMatrices;

        Texture2D* m_inputEquirectangularTexture2D;
        CubeTexture2D* m_outputCubeTexture2D;

    public:
        EnviromentMapRenderPass(Context* context, Texture2D* target);
        EnviromentMapRenderPass(const EnviromentMapRenderPass&) = default;
        virtual ~EnviromentMapRenderPass();

        void SetRenderArguments(Texture2D* srcTexture2D, CubeTexture2D* dstTexture2D);
        void Render(RenderPass* prev) override;

    private:
        void SetupShaders();
        void SetupBuffers();
        void SetupCubeMesh();
        void SetupCameraMatrices();

        void UpdateBuffers(Matrix4x4& matrix);
    };
}

#endif // CUBEMAPRENDERPASS_H
