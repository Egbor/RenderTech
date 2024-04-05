#include "Engine/Core/Render/Api/DX11/DX11SamplerState.h"
#include "Engine/Core/System/Exception/EngineException.h"

namespace Engine {
    D3D11_FILTER gFilterTable[] = {
        D3D11_FILTER_MIN_MAG_MIP_POINT,
        D3D11_FILTER_MIN_MAG_MIP_LINEAR,
        D3D11_FILTER_ANISOTROPIC
    };

    D3D11_TEXTURE_ADDRESS_MODE gAddressModeTable[] = {
        D3D11_TEXTURE_ADDRESS_WRAP,
        D3D11_TEXTURE_ADDRESS_MIRROR,
        D3D11_TEXTURE_ADDRESS_CLAMP,
        D3D11_TEXTURE_ADDRESS_BORDER,
        D3D11_TEXTURE_ADDRESS_MIRROR_ONCE
    };

    constexpr D3D11_SAMPLER_DESC GenerateD3D11SamplerDesc(const SamplerState& data) {
        D3D11_SAMPLER_DESC d3dSamplerDesc {};
        d3dSamplerDesc.AddressV = gAddressModeTable[INDEX_OF(data.address)];
        d3dSamplerDesc.AddressU = gAddressModeTable[INDEX_OF(data.address)];
        d3dSamplerDesc.AddressW = gAddressModeTable[INDEX_OF(data.address)];
        d3dSamplerDesc.Filter = gFilterTable[INDEX_OF(data.filter)];

        return d3dSamplerDesc;
    }

    DX11SamplerState::DX11SamplerState(const StateData& data, const IContext* context) {
        ComPtr<ID3D11Device> d3dDevice = dynamic_cast<const DX11Context*>(context)->GetD3D11Device();
        const D3D11_SAMPLER_DESC dxData = GenerateD3D11SamplerDesc(data.sdSampler);

        HRESULT hr = 0;
        if (FAILED(hr = d3dDevice->CreateSamplerState(&dxData, &m_state))) {
            throw EngineException("[DX11SamplerState] ID3D11Device::CreateSampler() failed");
        }
    }

    bool DX11SamplerState::Is(StateType type) const {
        return type == StateType::ST_SAMPLER;
    }

    ComPtr<ID3D11SamplerState> DX11SamplerState::GetD3D11SamplerState() const {
        return m_state;
    }
}
