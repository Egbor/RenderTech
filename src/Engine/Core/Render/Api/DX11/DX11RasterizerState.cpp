#include "Engine/Core/Render/Api/DX11/DX11RasterizerState.h"
#include "Engine/Core/Render/Api/DX11/DX11Context.h"
#include "Engine/Core/System/Exception/EngineException.h"

namespace Engine {
    D3D11_CULL_MODE gCullModeTable[] = {
        D3D11_CULL_NONE,
        D3D11_CULL_FRONT,
        D3D11_CULL_BACK
    };

    DX11RasterizerStateData::DX11RasterizerStateData()
        : m_d3dRasterizerDesc() {
        Reset();
    }

    void DX11RasterizerStateData::Reset() {
        m_d3dRasterizerDesc.FillMode = D3D11_FILL_SOLID;
        m_d3dRasterizerDesc.CullMode = D3D11_CULL_BACK;
        m_d3dRasterizerDesc.FrontCounterClockwise = FALSE;
        m_d3dRasterizerDesc.DepthBias = 0;
        m_d3dRasterizerDesc.SlopeScaledDepthBias = 0.0f;
        m_d3dRasterizerDesc.DepthBiasClamp = 0.0f;
        m_d3dRasterizerDesc.DepthClipEnable = TRUE;
        m_d3dRasterizerDesc.ScissorEnable = FALSE;
        m_d3dRasterizerDesc.MultisampleEnable = FALSE;
        m_d3dRasterizerDesc.AntialiasedLineEnable = FALSE;
    }

    void  DX11RasterizerStateData::SetCullMode(CullMode mode) {
        m_d3dRasterizerDesc.CullMode = gCullModeTable[INDEX_OF(mode)];
    }

    void DX11RasterizerStateData::SetDepthClipEnable(bool enable) {
        m_d3dRasterizerDesc.DepthClipEnable = enable;
    }

    const D3D11_RASTERIZER_DESC& DX11RasterizerStateData::GetDesc() const {
        return m_d3dRasterizerDesc;
    }

    DX11RasterizerState::DX11RasterizerState(const IStateData* data, const IContext* context) {
        Initialize(data, context);
    }

    void DX11RasterizerState::Initialize(const IStateData* data, const IContext* context) {
        ComPtr<ID3D11Device> d3dDevice = dynamic_cast<const DX11Context*>(context)->GetD3D11Device();
        const DX11RasterizerStateData* dxData = dynamic_cast<const DX11RasterizerStateData*>(data);

        HRESULT hr = 0;
        if (FAILED(hr = d3dDevice->CreateRasterizerState(&dxData->GetDesc(), &m_state))) {
            throw EngineException("[DX11RasterizerState] ID3D11Device::CreateRasterizerState() failed");
        }
    }

    ComPtr<ID3D11RasterizerState> DX11RasterizerState::GetD3D11RasterizerState() const {
        return m_state;
    }
}
