#include "Engine/Core/Render/Api/DX11/DX11DepthStencilState.h"
#include "Engine/Core/Render/Api/DX11/DX11Context.h"
#include "Engine/Core/System/Exception/EngineException.h"

namespace Engine {
    D3D11_COMPARISON_FUNC gComparisonFuncTable[] = {
        D3D11_COMPARISON_NEVER,
        D3D11_COMPARISON_LESS,
        D3D11_COMPARISON_EQUAL,
        D3D11_COMPARISON_LESS_EQUAL,
        D3D11_COMPARISON_GREATER,
        D3D11_COMPARISON_NOT_EQUAL,
        D3D11_COMPARISON_GREATER_EQUAL,
        D3D11_COMPARISON_ALWAYS,
    };

    D3D11_STENCIL_OP gStencilOpTable[] = {
        D3D11_STENCIL_OP_KEEP,
        D3D11_STENCIL_OP_ZERO,
        D3D11_STENCIL_OP_REPLACE,
        D3D11_STENCIL_OP_INCR_SAT,
        D3D11_STENCIL_OP_DECR_SAT,
        D3D11_STENCIL_OP_INVERT,
        D3D11_STENCIL_OP_INCR,
        D3D11_STENCIL_OP_DECR,
    };

    constexpr D3D11_DEPTH_STENCIL_DESC GenerateD3D11DepthStencilDesc(const DepthStencilState& data) {
        D3D11_DEPTH_STENCIL_DESC d3dDepthStencilDesc {};
        d3dDepthStencilDesc.DepthEnable = data.depthTestEnable;
        d3dDepthStencilDesc.DepthWriteMask = data.depthWriteEnable ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
        d3dDepthStencilDesc.DepthFunc = gComparisonFuncTable[INDEX_OF(data.depthComparisonFunction)];
        d3dDepthStencilDesc.StencilEnable = data.stencilTestEnable;
        d3dDepthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
        d3dDepthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
        d3dDepthStencilDesc.FrontFace.StencilFunc = gComparisonFuncTable[INDEX_OF(data.frontFace.stencilComparisonFunction)];
        d3dDepthStencilDesc.FrontFace.StencilDepthFailOp = gStencilOpTable[INDEX_OF(data.frontFace.stencilConditions.opStencilDepthFail)];
        d3dDepthStencilDesc.FrontFace.StencilPassOp = gStencilOpTable[INDEX_OF(data.frontFace.stencilConditions.opStencilPass)];
        d3dDepthStencilDesc.FrontFace.StencilFailOp = gStencilOpTable[INDEX_OF(data.frontFace.stencilConditions.opStencilFail)];
        d3dDepthStencilDesc.BackFace.StencilFunc = gComparisonFuncTable[INDEX_OF(data.backFace.stencilComparisonFunction)];
        d3dDepthStencilDesc.BackFace.StencilDepthFailOp = gStencilOpTable[INDEX_OF(data.backFace.stencilConditions.opStencilDepthFail)];
        d3dDepthStencilDesc.BackFace.StencilPassOp = gStencilOpTable[INDEX_OF(data.backFace.stencilConditions.opStencilPass)];
        d3dDepthStencilDesc.BackFace.StencilFailOp = gStencilOpTable[INDEX_OF(data.backFace.stencilConditions.opStencilFail)];

        return d3dDepthStencilDesc;
    }

    DX11DepthStencilState::DX11DepthStencilState(const StateData& data, IContext* context)  
        : StandaloneStateResource(context), m_stencilRef(data.sdDepthStencil.stencilRef) {
        ComPtr<ID3D11Device> d3dDevice = dynamic_cast<DX11Context*>(context)->GetD3D11Device();
        const D3D11_DEPTH_STENCIL_DESC dxData = GenerateD3D11DepthStencilDesc(data.sdDepthStencil);

        HRESULT hr = 0;
        if (FAILED(hr = d3dDevice->CreateDepthStencilState(&dxData, &m_state))) {
            throw EngineException("[DX11DepthStencilState] ID3D11Device::CreateDepthStencilState() failed");
        }
    }

    void DX11DepthStencilState::Bind() const {
        ComPtr<ID3D11DeviceContext> d3dContext = dynamic_cast<DX11Context*>(GetContext())->GetD3D11Context();
        d3dContext->OMSetDepthStencilState(m_state.Get(), m_stencilRef);
    }

    bool DX11DepthStencilState::Is(StateType type) const {
        return type == StateType::ST_DEPTH_STENCIL;
    }

    ComPtr<ID3D11DepthStencilState> DX11DepthStencilState::GetD3D11DepthStencilState() const {
        return m_state;
    }
}
