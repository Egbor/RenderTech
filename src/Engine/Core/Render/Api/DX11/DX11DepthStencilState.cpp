#include "Engine/Core/Render/Api/DX11/DX11DepthStencilState.h"
#include "Engine/Core/Render/Api/DX11/DX11Context.h"
#include "Engine/Core/System/Exception/EngineException.h"

namespace Engine {
    D3D11_COMPARISON_FUNC gComarisonFuncTable[] = {
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

    DX11DepthStencilState::DX11DepthStencilState()
        : m_d3dDepthStencilDesc(){
        Reset();
    }

    void DX11DepthStencilState::SetDepthTestComparisonFunction(ComparisonFunction func) {
        m_d3dDepthStencilDesc.DepthFunc = gComarisonFuncTable[INDEX_OF(func)];
    }

    void DX11DepthStencilState::SetStencilTestComparisonBackFunction(ComparisonFunction func) {
        m_d3dDepthStencilDesc.BackFace.StencilFunc = gComarisonFuncTable[INDEX_OF(func)];
    }

    void DX11DepthStencilState::SetStencilTestComparisonFrontFunction(ComparisonFunction func) {
        m_d3dDepthStencilDesc.FrontFace.StencilFunc = gComarisonFuncTable[INDEX_OF(func)];
    }

    void DX11DepthStencilState::SetStencilTestBackOperation(StencilConditions conditions) {
        m_d3dDepthStencilDesc.BackFace.StencilFailOp = gStencilOpTable[INDEX_OF(conditions.opStencilFail)];
        m_d3dDepthStencilDesc.BackFace.StencilDepthFailOp = gStencilOpTable[INDEX_OF(conditions.opStencilDepthFail)];
        m_d3dDepthStencilDesc.BackFace.StencilPassOp = gStencilOpTable[INDEX_OF(conditions.opStencilPass)];
    }

    void DX11DepthStencilState::SetStencilTestFrontOperation(StencilConditions conditions) {
        m_d3dDepthStencilDesc.FrontFace.StencilFailOp = gStencilOpTable[INDEX_OF(conditions.opStencilFail)];
        m_d3dDepthStencilDesc.FrontFace.StencilDepthFailOp = gStencilOpTable[INDEX_OF(conditions.opStencilDepthFail)];
        m_d3dDepthStencilDesc.FrontFace.StencilPassOp = gStencilOpTable[INDEX_OF(conditions.opStencilPass)];
    }

    void DX11DepthStencilState::SetDepthTestEnable(bool enable) {
        m_d3dDepthStencilDesc.DepthEnable = enable;
    }

    void DX11DepthStencilState::SetStencilTestEnable(bool enable) {
        m_d3dDepthStencilDesc.StencilEnable = enable;
    }

    void DX11DepthStencilState::SetDepthWriteEnable(bool enable) {
        m_d3dDepthStencilDesc.DepthWriteMask = enable ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
    }

    void DX11DepthStencilState::Reset() {
        m_d3dDepthStencilDesc.DepthEnable = TRUE;
        m_d3dDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        m_d3dDepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
        m_d3dDepthStencilDesc.StencilEnable = FALSE;
        m_d3dDepthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
        m_d3dDepthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
        m_d3dDepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
        m_d3dDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
        m_d3dDepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        m_d3dDepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        m_d3dDepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
        m_d3dDepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
        m_d3dDepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        m_d3dDepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    }

    ComPtr<ID3D11DepthStencilState> DX11DepthStencilState::GetD3D11DepthStencilState(ComPtr<ID3D11Device> d3dDevice) {
        ComPtr<ID3D11DepthStencilState> d3dDepthStencilState;

        HRESULT hr = 0;
        if (FAILED(hr = d3dDevice->CreateDepthStencilState(&m_d3dDepthStencilDesc, &d3dDepthStencilState))) {
            throw EngineException("[DX11DepthStencilState] ID3D11Device::CreateDepthStencilState() failed");
        }
        return d3dDepthStencilState;
    }
}
