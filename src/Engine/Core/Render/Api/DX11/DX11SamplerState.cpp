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

    DX11SamplerStateData::DX11SamplerStateData() {
        Reset();
    }

    void DX11SamplerStateData::Reset() {
        m_d3dSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        m_d3dSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
        m_d3dSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
        m_d3dSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
        m_d3dSamplerDesc.MinLOD = -D3D11_FLOAT32_MAX;
        m_d3dSamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
        m_d3dSamplerDesc.MipLODBias = 0.0f;
        m_d3dSamplerDesc.MaxAnisotropy = 1;
        m_d3dSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        m_d3dSamplerDesc.BorderColor[0] = 1.0f;
        m_d3dSamplerDesc.BorderColor[1] = 1.0f;
        m_d3dSamplerDesc.BorderColor[2] = 1.0f;
        m_d3dSamplerDesc.BorderColor[3] = 1.0f;
    }

    void DX11SamplerStateData::SetAddress(SamplerAddress address) {
        m_d3dSamplerDesc.AddressU = gAddressModeTable[INDEX_OF(address)];
        m_d3dSamplerDesc.AddressV = gAddressModeTable[INDEX_OF(address)];
        m_d3dSamplerDesc.AddressW = gAddressModeTable[INDEX_OF(address)];
    }

    void DX11SamplerStateData::SetFilter(SamplerFilter filter) {
        m_d3dSamplerDesc.Filter = gFilterTable[INDEX_OF(filter)];
    }

    const D3D11_SAMPLER_DESC& DX11SamplerStateData::GetD3D11Desc() const {
        return m_d3dSamplerDesc;
    }

    DX11SamplerState::DX11SamplerState(const IStateData* data, const IContext* context) {
        Initialize(data, context);
    }

    void DX11SamplerState::Initialize(const IStateData* data, const IContext* context) {
        ComPtr<ID3D11Device> d3dDevice = dynamic_cast<const DX11Context*>(context)->GetD3D11Device();
        const DX11SamplerStateData* dxData = dynamic_cast<const DX11SamplerStateData*>(data);

        HRESULT hr = 0;
        if (FAILED(hr = d3dDevice->CreateSamplerState(&dxData->GetD3D11Desc(), &m_state))) {
            throw EngineException("[DX11SamplerState] ID3D11Device::CreateSampler() failed");
        }
    }

    ComPtr<ID3D11SamplerState> DX11SamplerState::GetD3D11SamplerState() const {
        return m_state;
    }
}
