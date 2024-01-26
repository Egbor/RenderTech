#ifndef DX11RASTERIZERSTATE_H
#define DX11RASTERIZERSTATE_H

#include "Engine/Core/Render/Api/Interface/IStateResource.h"
#include "Engine/Core/Render/Api/DX11/DX11Context.h"

namespace Engine {
    class DX11RasterizerStateData : public IRasterizerStateData {
    private:
        D3D11_RASTERIZER_DESC m_d3dRasterizerDesc;

    public:
        DX11RasterizerStateData();
        virtual ~DX11RasterizerStateData() = default;

        void Reset() override;

        void SetCullMode(CullMode mode) override;
        void SetDepthClipEnable(bool enable) override;

        const D3D11_RASTERIZER_DESC& GetDesc() const;
    };

    class DX11RasterizerState : public IStateResourceData {
    public:
        DX11RasterizerState(const IStateData* data, const IContext* context);
        virtual ~DX11RasterizerState() = default;

        void Initialize(const IStateData* data, const IContext* context) override;

        ComPtr<ID3D11RasterizerState> GetD3D11RasterizerState() const;

    private:
        ComPtr<ID3D11RasterizerState> m_state;
    };
}

#endif // DX11RASTERIZERSTATE_H
