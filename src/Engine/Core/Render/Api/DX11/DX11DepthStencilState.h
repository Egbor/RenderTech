#ifndef DX11DEPTHSTENCILSTATE_H
#define DX11DEPTHSTENCILSTATE_H

#include "Engine/Core/Render/Api/Interface/IStateResource.h"
#include "Engine/Core/Render/Api/DX11/DX11Context.h"

namespace Engine {
    class DX11DepthStencilState : public AbstractDepthStencilState {
    private:
        D3D11_DEPTH_STENCIL_DESC m_d3dDepthStencilDesc;

    public:
        DX11DepthStencilState();
        virtual ~DX11DepthStencilState() = default;

        void Reset() override;

        void SetDepthTestComparisonFunction(ComparisonFunction func) override;
        void SetStencilTestComparisonBackFunction(ComparisonFunction func) override;
        void SetStencilTestComparisonFrontFunction(ComparisonFunction func) override;

        void SetStencilTestBackOperation(StencilConditions conditions) override;
        void SetStencilTestFrontOperation(StencilConditions conditions) override;

        void SetDepthTestEnable(bool enable) override;
        void SetStencilTestEnable(bool enable) override;
        void SetDepthWriteEnable(bool enable) override;

        ComPtr<ID3D11DepthStencilState> GetD3D11DepthStencilState(ComPtr<ID3D11Device> d3dDevice);
    };
}

#endif // DX11DEPTHSTENCILSTATE_H
