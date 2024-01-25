#ifndef DX11RASTERIZERSTATE_H
#define DX11RASTERIZERSTATE_H

#include "Engine/Core/Render/Api/Interface/IStateResource.h"
#include "Engine/Core/Render/Api/DX11/DX11Context.h"

namespace Engine {
    class DX11RasterizerState : public AbstaractRasterizerState {
    private:
        D3D11_RASTERIZER_DESC m_d3dRasterizerDesc;

    public:
        DX11RasterizerState();
        virtual ~DX11RasterizerState() = default;

        void Reset() override;

        void SetCullMode(CullMode mode) override;
        void SetDepthClipEnable(bool enable) override;

        ComPtr<ID3D11RasterizerState> GetD3D11RasterizerState(ComPtr<ID3D11Device> d3dDevice);
    };
}

#endif // DX11RASTERIZERSTATE_H
