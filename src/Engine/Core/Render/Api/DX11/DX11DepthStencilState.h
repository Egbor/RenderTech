#ifndef DX11DEPTHSTENCILSTATE_H
#define DX11DEPTHSTENCILSTATE_H

#include "Engine/Core/Render/Api/DX11/DX11Context.h"

namespace Engine {
    class DX11DepthStencilState {
    private:
        DX11Context* m_dxContext;

        UINT m_stencilRef;
        D3D11_DEPTH_STENCIL_DESC m_d3dDepthStencilDesc;

    public:
        DX11DepthStencilState(DX11Context* dxContext);
        DX11DepthStencilState(const DX11DepthStencilState&) = default;
        virtual ~DX11DepthStencilState() = default;

        //void SetDepthTestComparisonFunction(ComparisonFunction func) override;
        //void SetStencilTestComparisonBackFunction(ComparisonFunction func) override;
        //void SetStencilTestComparisonFrontFunction(ComparisonFunction func) override;

        //void SetStencilTestBackOperation(StencilConditions conditions) override;
        //void SetStencilTestFrontOperation(StencilConditions conditions) override;

        //void SetDepthTestEnable(bool enable) override;
        //void SetStencilTestEnable(bool enable) override;

        //void SetDepthWriteEnable(bool enable) override;
        //void SetStencilRef(UInt32 ref) override;

        //void Reset() override;

        //void Bind() override;
        //void UnBind() override;
    };
}

#endif // DX11DEPTHSTENCILSTATE_H
