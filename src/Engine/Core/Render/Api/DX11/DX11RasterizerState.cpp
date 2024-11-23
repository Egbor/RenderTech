#include "Engine/Core/Render/Api/DX11/DX11RasterizerState.h"
#include "Engine/Core/Render/Api/DX11/DX11Context.h"
#include "Engine/Core/System/Exception/EngineException.h"

namespace Engine {
    D3D11_CULL_MODE gCullModeTable[] = {
        D3D11_CULL_NONE,
        D3D11_CULL_FRONT,
        D3D11_CULL_BACK
    };

    constexpr D3D11_RASTERIZER_DESC GenerateD3D11RasterizerDesc(const RasterizerState& data) {
        D3D11_RASTERIZER_DESC d3dRasterizerDesc {};
        d3dRasterizerDesc.FillMode = D3D11_FILL_SOLID;
        d3dRasterizerDesc.CullMode = gCullModeTable[INDEX_OF(data.culling)];
        d3dRasterizerDesc.FrontCounterClockwise = false;
        d3dRasterizerDesc.DepthBias = 0;
        d3dRasterizerDesc.SlopeScaledDepthBias = 0.0f;
        d3dRasterizerDesc.DepthBiasClamp = 0.0f;
        d3dRasterizerDesc.DepthClipEnable = data.depthClipEnable;
        d3dRasterizerDesc.ScissorEnable = false;
        d3dRasterizerDesc.MultisampleEnable = false;
        d3dRasterizerDesc.AntialiasedLineEnable = false;

        return d3dRasterizerDesc;
    }

    DX11RasterizerState::DX11RasterizerState(const String& name, const StateData& data, IContext* context) 
        : StandaloneStateResource(name, context) {
        ComPtr<ID3D11Device> d3dDevice = dynamic_cast<const DX11Context*>(context)->GetD3D11Device();
        const D3D11_RASTERIZER_DESC dxData = GenerateD3D11RasterizerDesc(data.sdRasterizer);

        HRESULT hr = 0;
        if (FAILED(hr = d3dDevice->CreateRasterizerState(&dxData, &m_state))) {
            throw EngineException("[DX11RasterizerState] ID3D11Device::CreateRasterizerState() failed");
        }
    }

    void DX11RasterizerState::Bind() const {
        ComPtr<ID3D11DeviceContext> d3dContext = dynamic_cast<DX11Context*>(GetContext())->GetD3D11Context();
        d3dContext->RSSetState(m_state.Get());
    }

    bool DX11RasterizerState::Is(StateType type) const {
        return type == StateType::ST_RASTERIZER;
    }

    ComPtr<ID3D11RasterizerState> DX11RasterizerState::GetD3D11RasterizerState() const {
        return m_state;
    }
}
