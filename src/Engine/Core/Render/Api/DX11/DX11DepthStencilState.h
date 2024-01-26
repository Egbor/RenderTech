#ifndef DX11DEPTHSTENCILSTATE_H
#define DX11DEPTHSTENCILSTATE_H

#include "Engine/Core/Render/Api/Interface/IStateResource.h"
#include "Engine/Core/Render/Api/DX11/DX11Context.h"

namespace Engine {
    class DX11DepthStencilStateData : public IDepthStencilStateData {
    private:
        D3D11_DEPTH_STENCIL_DESC m_d3dDepthStencilDesc;

    public:
        DX11DepthStencilStateData();
        virtual ~DX11DepthStencilStateData() = default;

        void Reset() override;

        void SetDepthTestComparisonFunction(ComparisonFunction func) override;
        void SetStencilTestComparisonBackFunction(ComparisonFunction func) override;
        void SetStencilTestComparisonFrontFunction(ComparisonFunction func) override;

        void SetStencilTestBackOperation(StencilConditions conditions) override;
        void SetStencilTestFrontOperation(StencilConditions conditions) override;

        void SetDepthTestEnable(bool enable) override;
        void SetStencilTestEnable(bool enable) override;
        void SetDepthWriteEnable(bool enable) override;

        const D3D11_DEPTH_STENCIL_DESC& GetD3D11Desc() const;
    };

    class DX11DepthStencilState : public IStateResourceData {
    public:
        DX11DepthStencilState(const IStateData* data, const IContext* context);
        virtual ~DX11DepthStencilState() = default;

        void Initialize(const IStateData* data, const IContext* context) override;

        ComPtr<ID3D11DepthStencilState> GetD3D11DepthStencilState() const;

    private:
        ComPtr<ID3D11DepthStencilState> m_state;
    };
}

#endif // DX11DEPTHSTENCILSTATE_H
