#ifndef DX11SAMPLERSTATE_H
#define DX11SAMPLERSTATE_H

#include "Engine/Core/Render/Api/Interface/IStateResource.h"
#include "Engine/Core/Render/Api/DX11/DX11Context.h"

namespace Engine {
    class DX11SamplerState : public AbstractSamplerState {
    private:
        D3D11_SAMPLER_DESC m_d3dSamplerDesc;

    public:
        DX11SamplerState();
        virtual ~DX11SamplerState() = default;

        void Reset() override;

        void SetAddress(SamplerAddress address) override;
        void SetFilter(SamplerFilter filter) override;

        ComPtr<ID3D11SamplerState> GetD3D11SamplerState(ComPtr<ID3D11Device> d3dDevice);
    };
}

#endif // DX11SAMPLERSTATE_H
