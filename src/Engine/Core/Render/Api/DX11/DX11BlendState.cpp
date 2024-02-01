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

    DX11BlendStateData::DX11BlendStateData()
        : m_d3dBlendDesc() {
        Reset();
    }

    void DX11BlendStateData::Reset() {
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

    void DX11BlendStateData::SetSourceFactor(RenderOutput target, Blend factor) {
        m_d3dBlendDesc.RenderTarget[INDEX_OF(target)].SrcBlend = gBlendTable[INDEX_OF(factor)];
    }

    void DX11BlendStateData::SetDestinationFactor(RenderOutput target, Blend factor) {
        m_d3dBlendDesc.RenderTarget[INDEX_OF(target)].DestBlend = gBlendTable[INDEX_OF(factor)];
    }

    void DX11BlendStateData::SetOperation(RenderOutput target, BlendOperation operation) {
        m_d3dBlendDesc.RenderTarget[INDEX_OF(target)].BlendOp = gBlendOpTable[INDEX_OF(operation)];
    }

    void DX11BlendStateData::SetEnable(RenderOutput target, bool enable) {
        m_d3dBlendDesc.RenderTarget[INDEX_OF(target)].BlendEnable = enable;
    }

    const D3D11_BLEND_DESC& DX11BlendStateData::GetD3D11Desc() const {
        return m_d3dBlendDesc;
    }

    DX11BlendState::DX11BlendState(const IStateData* data, const IContext* context) {
        Initialize(data, context);
    }

    void DX11BlendState::Initialize(const IStateData* data, const IContext* context) {
        ComPtr<ID3D11Device> d3dDevice = dynamic_cast<const DX11Context*>(context)->GetD3D11Device();
        const DX11BlendStateData* dxData = dynamic_cast<const DX11BlendStateData*>(data);

        HRESULT hr = 0;
        if (FAILED(hr = d3dDevice->CreateBlendState(&dxData->GetD3D11Desc(), &m_state))) {
            throw new EngineException("[DX11BlendState] ID3D11Device::CreateBlendState() failed");
        }
    }

    ComPtr<ID3D11BlendState> DX11BlendState::GetD3D11BlendState() const {
        return m_state;
    }
}
