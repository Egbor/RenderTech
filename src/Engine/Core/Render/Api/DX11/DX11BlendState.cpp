#include "Engine/Core/Render/Api/DX11/DX11BlendState.h"
#include "Engine/Core/Render/Api/DX11/DX11Context.h"
#include "Engine/Core/System/Exception/EngineException.h"

namespace Engine {
    D3D11_BLEND gBlendTable[] = {
        D3D11_BLEND_ZERO,
        D3D11_BLEND_ONE,
        D3D11_BLEND_SRC_COLOR,
        D3D11_BLEND_INV_SRC_COLOR,
        D3D11_BLEND_SRC_ALPHA,
        D3D11_BLEND_INV_SRC_ALPHA,
        D3D11_BLEND_DEST_ALPHA,
        D3D11_BLEND_INV_DEST_ALPHA,
        D3D11_BLEND_DEST_COLOR,
        D3D11_BLEND_INV_DEST_COLOR,
        D3D11_BLEND_SRC_ALPHA_SAT,
        D3D11_BLEND_BLEND_FACTOR,
        D3D11_BLEND_INV_BLEND_FACTOR,
        D3D11_BLEND_SRC1_COLOR,
        D3D11_BLEND_INV_SRC1_COLOR,
        D3D11_BLEND_SRC1_ALPHA,
        D3D11_BLEND_INV_SRC1_ALPHA
    };

    D3D11_BLEND_OP gBlendOpTable[] = {
        D3D11_BLEND_OP_ADD,
        D3D11_BLEND_OP_SUBTRACT,
        D3D11_BLEND_OP_REV_SUBTRACT,
        D3D11_BLEND_OP_MIN,
        D3D11_BLEND_OP_MAX
    };

    constexpr D3D11_BLEND_DESC GenerateD3D11BlendDesc(const BlendState& data) {
        D3D11_BLEND_DESC d3dBlendDesc {};
        d3dBlendDesc.AlphaToCoverageEnable = FALSE;
        d3dBlendDesc.IndependentBlendEnable = FALSE;

        for (Int32 i = 0; i < ARRAYSIZE(data.targets); i++) {
            d3dBlendDesc.RenderTarget[i].BlendEnable = data.targets[i].blendEnable;
            d3dBlendDesc.RenderTarget[i].SrcBlend = gBlendTable[INDEX_OF(data.targets[i].blendSrc)];
            d3dBlendDesc.RenderTarget[i].DestBlend = gBlendTable[INDEX_OF(data.targets[i].blendDst)];
            d3dBlendDesc.RenderTarget[i].BlendOp = gBlendOpTable[INDEX_OF(data.targets[i].blendOperation)];
            d3dBlendDesc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
            d3dBlendDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
            d3dBlendDesc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
            d3dBlendDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        }

        return d3dBlendDesc;
    }

    DX11BlendState::DX11BlendState(const String& name, const StateData& data, IContext* context) 
        : StandaloneStateResource(name, context), m_blendFactor{1.0f, 1.0f, 1.0f, 1.0f}, m_sampleMask(0xFFFFFFFF) {
        ComPtr<ID3D11Device> d3dDevice = dynamic_cast<DX11Context*>(context)->GetD3D11Device();
        const D3D11_BLEND_DESC dxData = GenerateD3D11BlendDesc(data.sdBlend);

        HRESULT hr = 0;
        if (FAILED(hr = d3dDevice->CreateBlendState(&dxData, &m_state))) {
            throw new EngineException("[DX11BlendState] ID3D11Device::CreateBlendState() failed");
        }
    }

    void DX11BlendState::Bind() const {
        ComPtr<ID3D11DeviceContext> d3dContext = dynamic_cast<DX11Context*>(GetContext())->GetD3D11Context();
        d3dContext->OMSetBlendState(m_state.Get(), m_blendFactor, m_sampleMask);
    }

    bool DX11BlendState::Is(StateType type) const {
        return type == StateType::ST_BLEND;
    }

    ComPtr<ID3D11BlendState> DX11BlendState::GetD3D11BlendState() const {
        return m_state;
    }
}
