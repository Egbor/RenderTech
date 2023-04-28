#include "Engine/Core/Render/Api/DX11/Binder/DX11RenderTargetBinder.h"

namespace Engine {
    DX11RenderTargetBinder::DX11RenderTargetBinder(DX11Context* context)
        : m_dxContext(context), m_d3dRenderTargets(RENDER_OUTPUT_COUNT) {

    }

    void DX11RenderTargetBinder::Bind() {
        ComPtr<ID3D11DeviceContext> d3dContext = m_dxContext->GetD3D11Context();
        ID3D11RenderTargetView* d3dRenderTargetViews[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT] = { nullptr };
        ID3D11DepthStencilView* d3dDepthStencilView = nullptr;

        for (int i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; i++) {
            d3dRenderTargetViews[i] = static_cast<ID3D11RenderTargetView*>(m_d3dRenderTargets[i].Get());
        }
        d3dDepthStencilView = static_cast<ID3D11DepthStencilView*>(m_d3dRenderTargets[static_cast<Int32>(RenderOutput::RO_DEPTH)].Get());

        d3dContext->OMSetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, d3dRenderTargetViews, d3dDepthStencilView);
    }

    void DX11RenderTargetBinder::UnBind() {
        ComPtr<ID3D11DeviceContext> d3dContext = m_dxContext->GetD3D11Context();
        d3dContext->OMSetRenderTargets(0, NULL, NULL);
    }

    void DX11RenderTargetBinder::AttachRenderItem(RenderOutput id, DX11Target* item) {
        if (item == nullptr) {
            DetachRenderItem(id);
            return;
        }
        m_d3dRenderTargets[static_cast<Int32>(id)] = item->GetD3D11View();
    }

    void DX11RenderTargetBinder::DetachRenderItem(RenderOutput id) {
        m_d3dRenderTargets[static_cast<Int32>(id)] = nullptr;
    }
}
