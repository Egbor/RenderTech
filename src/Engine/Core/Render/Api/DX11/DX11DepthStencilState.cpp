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

    DX11DepthStencilState::DX11DepthStencilState(DX11Context* dxContext)
        : m_dxContext(dxContext), m_stencilRef(0) {
        //Reset();
    }

    //void DX11DepthStencilState::SetDepthTestComparisonFunction(ComparisonFunction func) {
    //    UInt32 tableId = static_cast<UInt32>(func);
    //    m_d3dDepthStencilDesc.DepthFunc = gComarisonFuncTable[tableId];
    //}

    //void DX11DepthStencilState::SetStencilTestComparisonBackFunction(ComparisonFunction func) {
    //    UInt32 tableId = static_cast<UInt32>(func);
    //    m_d3dDepthStencilDesc.BackFace.StencilFunc = gComarisonFuncTable[tableId];
    //}

    //void DX11DepthStencilState::SetStencilTestComparisonFrontFunction(ComparisonFunction func) {
    //    UInt32 tableId = static_cast<UInt32>(func);
    //    m_d3dDepthStencilDesc.FrontFace.StencilFunc = gComarisonFuncTable[tableId];
    //}

    //void DX11DepthStencilState::SetStencilTestBackOperation(StencilConditions conditions) {
    //    UInt32 stencilFaildTableId = static_cast<UInt32>(conditions.opStencilFail);
    //    UInt32 stencilDepthFailTableId = static_cast<UInt32>(conditions.opStencilDepthFail);
    //    UInt32 stencilPassTableId = static_cast<UInt32>(conditions.opStencilPass);

    //    m_d3dDepthStencilDesc.BackFace.StencilFailOp = gStencilOpTable[stencilFaildTableId];
    //    m_d3dDepthStencilDesc.BackFace.StencilDepthFailOp = gStencilOpTable[stencilDepthFailTableId];
    //    m_d3dDepthStencilDesc.BackFace.StencilPassOp = gStencilOpTable[stencilPassTableId];
    //}

    //void DX11DepthStencilState::SetStencilTestFrontOperation(StencilConditions conditions) {
    //    UInt32 stencilFaildTableId = static_cast<UInt32>(conditions.opStencilFail);
    //    UInt32 stencilDepthFailTableId = static_cast<UInt32>(conditions.opStencilDepthFail);
    //    UInt32 stencilPassTableId = static_cast<UInt32>(conditions.opStencilPass);

    //    m_d3dDepthStencilDesc.FrontFace.StencilFailOp = gStencilOpTable[stencilFaildTableId];
    //    m_d3dDepthStencilDesc.FrontFace.StencilDepthFailOp = gStencilOpTable[stencilDepthFailTableId];
    //    m_d3dDepthStencilDesc.FrontFace.StencilPassOp = gStencilOpTable[stencilPassTableId];
    //}

    //void DX11DepthStencilState::SetDepthTestEnable(bool enable) {
    //    m_d3dDepthStencilDesc.DepthEnable = enable;
    //}

    //void DX11DepthStencilState::SetStencilTestEnable(bool enable) {
    //    m_d3dDepthStencilDesc.StencilEnable = enable;
    //}

    //void DX11DepthStencilState::SetDepthWriteEnable(bool enable) {
    //    m_d3dDepthStencilDesc.DepthWriteMask = enable ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
    //}

    //void DX11DepthStencilState::SetStencilRef(UInt32 ref) {
    //    m_stencilRef = ref;
    //}

    //void DX11DepthStencilState::Bind() {
    //    HRESULT hr = 0;
    //    ComPtr<ID3D11Device> d3dDevice = m_dxContext->GetD3D11Device();
    //    ComPtr<ID3D11DeviceContext> d3dContext = m_dxContext->GetD3D11Context();

    //    ComPtr<ID3D11DepthStencilState> d3dDepthStencilState;
    //    if (FAILED(hr = d3dDevice->CreateDepthStencilState(&m_d3dDepthStencilDesc, &d3dDepthStencilState))) {
    //        throw new EngineException("[DX11DepthStencilState] ID3D11Device::CreateDepthStencilState() failed");
    //    }
    //    d3dContext->OMSetDepthStencilState(d3dDepthStencilState.Get(), m_stencilRef);
    //}

    //void DX11DepthStencilState::Reset() {
    //    m_stencilRef = 0;
    //    m_d3dDepthStencilDesc.DepthEnable = TRUE;
    //    m_d3dDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    //    m_d3dDepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
    //    m_d3dDepthStencilDesc.StencilEnable = FALSE;
    //    m_d3dDepthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
    //    m_d3dDepthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
    //    m_d3dDepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    //    m_d3dDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    //    m_d3dDepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    //    m_d3dDepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    //    m_d3dDepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    //    m_d3dDepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    //    m_d3dDepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    //    m_d3dDepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    //}

    //void DX11DepthStencilState::UnBind() {
    //    ComPtr<ID3D11DeviceContext> d3dContext = m_dxContext->GetD3D11Context();
    //    d3dContext->OMSetDepthStencilState(NULL, 0);
    //}
}
