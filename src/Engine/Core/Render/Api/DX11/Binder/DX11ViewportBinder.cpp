#include "Engine/Core/Render/Api/DX11/Binder/DX11ViewportBinder.h"

namespace Engine {
    DX11ViewportBinder::DX11ViewportBinder(DX11Context* context)
        : m_dxContext(context), m_viewports(RENDER_OUTPUT_COUNT) {

    }

    void DX11ViewportBinder::Bind() {
        ComPtr<ID3D11DeviceContext> d3dContext = m_dxContext->GetD3D11Context();
        D3D11_VIEWPORT d3dViewports[RENDER_OUTPUT_COUNT];
        Int32 d3dViewportsCount = 0;

        for (RenderViewport* viewport : m_viewports) {
            if (viewport != nullptr) {
                d3dViewports[d3dViewportsCount++] = {
                    static_cast<FLOAT>(viewport->topLeftX),
                    static_cast<FLOAT>(viewport->topLeftY),
                    static_cast<FLOAT>(viewport->width),
                    static_cast<FLOAT>(viewport->height),
                    0.0f,
                    1.0f
                };
            }
        }

        d3dContext->RSSetViewports(d3dViewportsCount, d3dViewports);
    }

    void DX11ViewportBinder::UnBind() {
        ComPtr<ID3D11DeviceContext> d3dContext = m_dxContext->GetD3D11Context();
        d3dContext->RSSetViewports(0, NULL);
    }

    void DX11ViewportBinder::AttachRenderItem(UInt32 id, RenderViewport* item) {
        if (m_viewports[id] != nullptr) {
            DetachRenderItem(id);
        }
        m_viewports[id] = item;
    }

    void DX11ViewportBinder::DetachRenderItem(UInt32 id) {
        m_viewports[id] = nullptr;
    }

    void DX11ViewportBinder::AttachRenderItem(Int32 id, void* item) {
        AttachRenderItem(static_cast<UInt32>(id), reinterpret_cast<RenderViewport*>(item));
    }

    void DX11ViewportBinder::DetachRenderItem(Int32 id) {
        DetachRenderItem(static_cast<UInt32>(id));
    }
}
