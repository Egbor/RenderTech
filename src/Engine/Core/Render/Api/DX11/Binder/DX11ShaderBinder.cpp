#include "Engine/Core/Render/Api/DX11/Binder/DX11ShaderBinder.h"

namespace Engine {
    DX11ShaderBinder::DX11ShaderBinder(DX11Context* context)
        : m_dxContext(context), m_dxShaders(RENDER_STAGE_COUNT) {

    }

    void DX11ShaderBinder::Bind() {
        for (DX11Shader* shader : m_dxShaders) {
            if (shader != nullptr) {
                shader->Bind(m_dxContext);
            }
        }
    }

    void DX11ShaderBinder::UnBind() {
        for (DX11Shader* shader : m_dxShaders) {
            if (shader != nullptr) {
                shader->UnBind(m_dxContext);
            }
        }
    }

    void DX11ShaderBinder::AttachRenderItem(RenderStage id, DX11Shader* item) {
        if (m_dxShaders[static_cast<Int32>(id)] != nullptr) {
            DetachRenderItem(id);
        }
        m_dxShaders[static_cast<Int32>(id)] = item;
    }

    void DX11ShaderBinder::DetachRenderItem(RenderStage id) {
        m_dxShaders[static_cast<Int32>(id)] = nullptr;
    }
}
