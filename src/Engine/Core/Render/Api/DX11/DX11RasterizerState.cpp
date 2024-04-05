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
        d3dRasterizerDesc.CullMode = gCullModeTable[INDEX_OF(data.culling)];
        d3dRasterizerDesc.DepthClipEnable = data.depthClipEnable;

        return d3dRasterizerDesc;
    }

    DX11RasterizerState::DX11RasterizerState(const StateData& data, const IContext* context) {
        ComPtr<ID3D11Device> d3dDevice = dynamic_cast<const DX11Context*>(context)->GetD3D11Device();
        const D3D11_RASTERIZER_DESC dxData = GenerateD3D11RasterizerDesc(data.sdRasterizer);

        HRESULT hr = 0;
        if (FAILED(hr = d3dDevice->CreateRasterizerState(&dxData, &m_state))) {
            throw EngineException("[DX11RasterizerState] ID3D11Device::CreateRasterizerState() failed");
        }
    }

    void DX11RasterizerState::Bind(ComPtr<ID3D11DeviceContext> d3dContext) {
        d3dContext->RSSetState(m_state.Get());
    }

    bool DX11RasterizerState::Is(StateType type) const {
        return type == StateType::ST_RASTERIZER;
    }

    ComPtr<ID3D11RasterizerState> DX11RasterizerState::GetD3D11RasterizerState() const {
        return m_state;
    }
}
