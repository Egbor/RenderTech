#include "Engine/Core/Render/Api/DX11/DX11SamplerState.h"
#include "Engine/Core/System/Exception/EngineException.h"

namespace Engine {
    D3D11_TEXTURE_ADDRESS_MODE gAddressModeTable[] = {
        D3D11_TEXTURE_ADDRESS_WRAP,
        D3D11_TEXTURE_ADDRESS_MIRROR,
        D3D11_TEXTURE_ADDRESS_CLAMP,
        D3D11_TEXTURE_ADDRESS_BORDER,
        D3D11_TEXTURE_ADDRESS_MIRROR_ONCE
    };

    DX11SamplerState::DX11SamplerState(DX11Context* dxContext)
        : m_dxContext(dxContext) {
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

    void DX11SamplerState::SetAddress(SamplerAddress address) {
        UInt32 addressId = static_cast<UInt32>(address);
        m_d3dSamplerDesc.AddressU = gAddressModeTable[addressId];
        m_d3dSamplerDesc.AddressV = gAddressModeTable[addressId];
        m_d3dSamplerDesc.AddressW = gAddressModeTable[addressId];
    }

    void DX11SamplerState::Bind() {
        HRESULT hr = 0;
        ComPtr<ID3D11Device> d3dDevice = m_dxContext->GetD3D11Device();
        ComPtr<ID3D11DeviceContext> d3dContext = m_dxContext->GetD3D11Context();

        ComPtr<ID3D11SamplerState> d3dSamplerState;
        if (FAILED(hr = d3dDevice->CreateSamplerState(&m_d3dSamplerDesc, &d3dSamplerState))) {
            throw new EngineException("[DX11SamplerState] ID3D11Device::CreateSampler() failed");
        }
        d3dContext->PSSetSamplers(0, 1, d3dSamplerState.GetAddressOf());
    }

    void DX11SamplerState::UnBind() {
        ComPtr<ID3D11DeviceContext> d3dContext = m_dxContext->GetD3D11Context();
        d3dContext->PSSetSamplers(0, 0, nullptr);
    }
}
