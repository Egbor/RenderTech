#ifndef DX11SAMPLERSTATE_H
#define DX11SAMPLERSTATE_H

#include "Engine/Core/Render/Api/Interface/IStateResource.h"
#include "Engine/Core/Render/Api/DX11/DX11Context.h"

namespace Engine {
    class DX11SamplerStateData : public ISamplerStateData {
    private:
        D3D11_SAMPLER_DESC m_d3dSamplerDesc;

    public:
        DX11SamplerStateData();
        virtual ~DX11SamplerStateData() = default;

        void Reset() override;

        void SetAddress(SamplerAddress address) override;
        void SetFilter(SamplerFilter filter) override;

        const D3D11_SAMPLER_DESC& GetD3D11Desc() const;
    };

    class DX11SamplerState : public IStateResourceData {
    public:
        DX11SamplerState(const IStateData* data, const IContext* context);
        virtual ~DX11SamplerState() = default;

        void Initialize(const IStateData* data, const IContext* context) override;

        ComPtr<ID3D11SamplerState> GetD3D11SamplerState() const;

    private:
        ComPtr<ID3D11SamplerState> m_state;
    };
}

#endif // DX11SAMPLERSTATE_H
