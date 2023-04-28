#ifndef DX11SHADERBINDER_H
#define DX11SHADERBINDER_H

#include "Engine/Core/Render/Api/RenderBinder.h"
#include "Engine/Core/Render/Api/DX11/DX11Shader.h"

namespace Engine {
    class DX11ShaderBinder : public RenderBinder<RenderStage, DX11Shader> {
    private:
        DX11Context* m_dxContext;
        Array<DX11Shader*> m_dxShaders;

    public:
        DX11ShaderBinder(DX11Context* context);
        DX11ShaderBinder(const DX11ShaderBinder&) = default;
        virtual ~DX11ShaderBinder() = default;

        void Bind() override;
        void UnBind() override;

        void AttachRenderItem(RenderStage id, DX11Shader* item) override;
        void DetachRenderItem(RenderStage id) override;
    };
}

#endif // DX11SHADERBINDER_H
