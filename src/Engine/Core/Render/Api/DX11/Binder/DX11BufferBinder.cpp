#include "Engine/Core/Render/Api/DX11/Binder/DX11BufferBinder.h"

namespace Engine {
    DX11BufferBinder::DX11BufferBinder(DX11Context* context)
        : m_dxContext(context), m_dxBuffers(RENDER_BUFFER_COUNT) {

    }

    void DX11BufferBinder::Bind() {
        for (DX11Buffer* buffer : m_dxBuffers) {
            if (buffer != nullptr) {
                buffer->Bind(m_dxContext);
            }
        }
    }

    void DX11BufferBinder::UnBind() {
        for (DX11Buffer* buffer : m_dxBuffers) {
            if (buffer != nullptr) {
                buffer->UnBind(m_dxContext);
            }
        }
    }

    void DX11BufferBinder::AttachRenderItem(RenderBuffer id, DX11Buffer* item) {
        if (m_dxBuffers[static_cast<Int32>(id)] != nullptr) {
            DetachRenderItem(id);
        }
        m_dxBuffers[static_cast<Int32>(id)] = item;
    }

    void DX11BufferBinder::DetachRenderItem(RenderBuffer id) {
        m_dxBuffers[static_cast<Int32>(id)] = nullptr;
    }

    void DX11BufferBinder::AttachRenderItem(Int32 id, void* item) {
        AttachRenderItem(static_cast<RenderBuffer>(id), reinterpret_cast<DX11Buffer*>(item));
    }

    void DX11BufferBinder::DetachRenderItem(Int32 id) {
        DetachRenderItem(static_cast<RenderBuffer>(id));
    }
}
