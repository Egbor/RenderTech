#ifndef DX11RENDERTARGETBINDER_H
#define DX11RENDERTARGETBINDER_H

#include "Engine/Core/Render/Api/RenderBinder.h"
#include "Engine/Core/Render/Api/DX11/DX11Target.h"

namespace Engine {
    class DX11RenderTargetBinder : public RenderBinder<RenderOutput, DX11Target> {
    private:
        DX11Context* m_dxContext;
        Array<ComPtr<ID3D11View>> m_d3dRenderTargets;

    public:
        DX11RenderTargetBinder(DX11Context* context);
        DX11RenderTargetBinder(const DX11RenderTargetBinder&) = default;
        virtual ~DX11RenderTargetBinder() = default;

        void Bind() override;
        void UnBind() override;

        void AttachRenderItem(RenderOutput id, DX11Target* item) override;
        void DetachRenderItem(RenderOutput id) override;
    };
}

#endif // DX11RENDERTARGETBINDER_H
