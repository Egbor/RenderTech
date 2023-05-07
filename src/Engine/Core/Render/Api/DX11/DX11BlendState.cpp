#include "Engine/Core/Render/Api/DX11/DX11BlendState.h"
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

    DX11BlendState::DX11BlendState(DX11Context* dxContext)
        : m_dxContext(dxContext), m_blendFactor({1.0f, 1.0f, 1.0f, 1.0f}) {
        Reset();
    }

    void DX11BlendState::SetSourceFactor(RenderOutput target, Blend factor) {
        UInt32 targetId = static_cast<UInt32>(target);
        UInt32 blendId = static_cast<UInt32>(factor);
        m_d3dBlendDesc.RenderTarget[targetId].SrcBlend = gBlendTable[blendId];
    }

    void DX11BlendState::SetDestinationFactor(RenderOutput target, Blend factor) {
        UInt32 targetId = static_cast<UInt32>(target);
        UInt32 blendId = static_cast<UInt32>(factor);
        m_d3dBlendDesc.RenderTarget[targetId].DestBlend = gBlendTable[blendId];
    }

    void DX11BlendState::SetOperation(RenderOutput target, BlendOperation operation) {
        UInt32 targetId = static_cast<UInt32>(target);
        UInt32 operationId = static_cast<UInt32>(operation);
        m_d3dBlendDesc.RenderTarget[targetId].BlendOp = gBlendOpTable[operationId];
    }

    void DX11BlendState::SetEnable(RenderOutput target, bool enable) {
        UInt32 targetId = static_cast<UInt32>(target);
        m_d3dBlendDesc.RenderTarget[targetId].BlendEnable = enable;

    }

    void DX11BlendState::Reset() {
        m_d3dBlendDesc.AlphaToCoverageEnable = FALSE;
        m_d3dBlendDesc.IndependentBlendEnable = FALSE;
        m_d3dBlendDesc.RenderTarget[0].BlendEnable = FALSE;
        m_d3dBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
        m_d3dBlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
        m_d3dBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        m_d3dBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        m_d3dBlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
        m_d3dBlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        m_d3dBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    }

    void DX11BlendState::Bind() {
        HRESULT hr = 0;
        ComPtr<ID3D11Device> d3dDevice = m_dxContext->GetD3D11Device();
        ComPtr<ID3D11DeviceContext> d3dContext = m_dxContext->GetD3D11Context();

        ComPtr<ID3D11BlendState> d3dBlendState;
        if (FAILED(hr = d3dDevice->CreateBlendState(&m_d3dBlendDesc, &d3dBlendState))) {
            throw new EngineException("[DX11BlendState] ID3D11Device::CreateBlendState() failed");
        }
        d3dContext->OMSetBlendState(d3dBlendState.Get(), m_blendFactor.data(), 0xFFFFFFFF);
    }

    void DX11BlendState::UnBind() {
        ComPtr<ID3D11DeviceContext> d3dContext = m_dxContext->GetD3D11Context();
        d3dContext->OMSetBlendState(NULL, NULL, 0xFFFFFFFF);
    }
}
