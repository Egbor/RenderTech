#include "Engine/Rendering/ShaderDescription.h"

namespace Engine {
    ShaderDescription::ShaderDescription()
        : m_byteCode() {

    }

    ShaderHandler ShaderDescription::Create(Size length) {
        m_byteCode.resize(length);
        return reinterpret_cast<ShaderHandler>(m_byteCode.data());
    }

    ShaderInfo ShaderDescription::GetShaderInfo() {
        return { m_byteCode.data(), m_byteCode.size() };
    }
}
