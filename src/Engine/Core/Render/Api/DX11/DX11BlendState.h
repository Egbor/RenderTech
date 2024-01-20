#ifndef DX11BLENDSTATE_H
#define DX11BLENDSTATE_H

#include "Engine/Core/Render/Api/DX11/DX11Context.h"

namespace Engine {
    class DX11BlendState {
    private:
        DX11Context* m_dxContext;

        Array<Float> m_blendFactor;
        D3D11_BLEND_DESC m_d3dBlendDesc;

    public:
        DX11BlendState(DX11Context* dxContext);
        DX11BlendState(const DX11BlendState&) = default;
        virtual ~DX11BlendState() = default;

        //void SetSourceFactor(RenderOutput target, Blend factor) override;
        //void SetDestinationFactor(RenderOutput target, Blend factor) override;
        //void SetOperation(RenderOutput target, BlendOperation operation) override;

        //void SetEnable(RenderOutput target, bool enable) override;

        //void Reset() override;

        //void Bind() override;
        //void UnBind() override;
    };
}

#endif // DX11BLENDSTATE_H
