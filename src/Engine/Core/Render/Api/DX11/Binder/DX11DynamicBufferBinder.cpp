#include "Engine/Core/Render/Api/DX11/Binder/DX11DynamicBufferBinder.h"

namespace Engine {
    DX11DynamicBufferBinder::DX11DynamicBufferBinder(DX11Context* context)
        : m_dxContext(context), m_dxBuffers(15) {

    }

    void DX11DynamicBufferBinder::Bind() {
        for (int i = 0; i < m_dxBuffers.size(); i++) {
            if (m_dxBuffers[i] != nullptr) {
                m_dxBuffers[i]->Bind(m_dxContext, i);
            }
        }
    }

    void DX11DynamicBufferBinder::UnBind() {
        for (int i = 0; i < m_dxBuffers.size(); i++) {
            if (m_dxBuffers[i] != nullptr) {
                m_dxBuffers[i]->UnBind(m_dxContext, i);
            }
        }
    }

    void DX11DynamicBufferBinder::AttachRenderItem(RenderDynamicBuffer id, DX11DynamicBuffer* buffer) {
        if (m_dxBuffers[static_cast<Int32>(id)] != nullptr) {
            DetachRenderItem(id);
        }
        m_dxBuffers[static_cast<Int32>(id)] = buffer;
    }

    void DX11DynamicBufferBinder::DetachRenderItem(RenderDynamicBuffer id) {
        m_dxBuffers[static_cast<Int32>(id)] = nullptr;
    }
}
