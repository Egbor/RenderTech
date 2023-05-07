#ifndef DX11SAMPLERSTATE_H
#define DX11SAMPLERSTATE_H

#include "Engine/Core/Render/Api/State/SamplerState.h"
#include "Engine/Core/Render/Api/DX11/DX11Context.h"

namespace Engine {
    class DX11SamplerState : public SamplerState {
    private:
        DX11Context* m_dxContext;

        D3D11_SAMPLER_DESC m_d3dSamplerDesc;

    public:
        DX11SamplerState(DX11Context* dxContext);
        DX11SamplerState(const DX11SamplerState&) = default;
        virtual ~DX11SamplerState() = default;

        void SetAddress(SamplerAddress address) override;

        void Reset() override;

        void Bind() override;
        void UnBind() override;
    };
}

#endif // DX11SAMPLERSTATE_H
