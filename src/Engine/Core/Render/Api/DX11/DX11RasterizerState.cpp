#include "Engine/Core/Render/Api/DX11/DX11RasterizerState.h"
#include "Engine/Core/Render/Api/DX11/DX11Context.h"
#include "Engine/Core/System/Exception/EngineException.h"

namespace Engine {
    D3D11_CULL_MODE gCullModeTable[] = {
        D3D11_CULL_NONE,
        D3D11_CULL_FRONT,
        D3D11_CULL_BACK
    };

    DX11RasterizerState::DX11RasterizerState(DX11Context* dxContext)
        : m_dxContext(dxContext) {
        Reset();
    }

    void  DX11RasterizerState::SetCullMode(CullMode mode) {
        UInt32 modeId = static_cast<UInt32>(mode);
        m_d3dRasterizerDesc.CullMode = gCullModeTable[modeId];
    }

    void DX11RasterizerState::SetDepthClipEnable(bool enable) {
        m_d3dRasterizerDesc.DepthClipEnable = enable;
    }

    void DX11RasterizerState::Reset() {
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

    void DX11RasterizerState::Bind() {
        HRESULT hr = 0;
        ComPtr<ID3D11Device> d3dDevice = m_dxContext->GetD3D11Device();
        ComPtr<ID3D11DeviceContext> d3dContext = m_dxContext->GetD3D11Context();

        ComPtr<ID3D11RasterizerState> d3dRasterizerState;
        if (FAILED(hr = d3dDevice->CreateRasterizerState(&m_d3dRasterizerDesc, &d3dRasterizerState))) {
            throw new EngineException("[DX11RasterizerState] ID3D11Device::CreateRasterizerState() failed");
        }
        d3dContext->RSSetState(d3dRasterizerState.Get());
    }

    void DX11RasterizerState::UnBind() {
        ComPtr<ID3D11DeviceContext> d3dContext = m_dxContext->GetD3D11Context();
        d3dContext->RSSetState(NULL);
    }
}
