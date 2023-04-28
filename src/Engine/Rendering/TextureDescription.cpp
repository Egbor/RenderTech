#include "Engine/Rendering/TextureDescription.h"
#include "Engine/Core/System/Exception/EngineException.h"

namespace Engine {
    constexpr static Int32 ToPitch(Int32 width, TextureFormat format) {
        switch (format) {
        case TextureFormat::TF_R32G32B32A32_FLOAT:
            return width * 16;
        case TextureFormat::TF_R32_INT:
        case TextureFormat::TF_R32_UINT:
        case TextureFormat::TF_R32_FLOAT:
        case TextureFormat::TF_R24G8_BMP:
        case TextureFormat::TF_R8G8B8A8_BMP:
        case TextureFormat::TF_B8G8R8A8_BMP:
        case TextureFormat::TF_R8G8B8A8_BMP_sRGB:
        case TextureFormat::TF_B8G8R8A8_BMP_sRGB:
            return width * 4;
        case TextureFormat::TF_R16_INT:
        case TextureFormat::TF_R16_UINT:
        case TextureFormat::TF_R8G8_BMP:
            return width * 2;
        case TextureFormat::TF_R8_BMP:
            return width * 1;
        case TextureFormat::TF_UNKNOWN:
            break;
        }
        return 0;
    }

    TextureDescription::TextureDescription()
        : m_width(0), m_height(0),
          m_format(TextureFormat::TF_UNKNOWN), m_data() {

    }

    TextureHandler TextureDescription::Create(Int32 width, Int32 height, TextureFormat format, const void* data) {
        m_width = width;
        m_height = height;
        m_format = format;
        m_data.resize(height * ToPitch(width, format));
        memcpy(m_data.data(), data, m_data.size());
        return reinterpret_cast<TextureHandler>(m_data.data());
    }

    TextureInfo TextureDescription::GetTextureInfo() {
       // return { static_cast<UInt32>(m_width), static_cast<UInt32>(m_height), m_format, m_data.data() };
        throw EngineException("[TextureDescription] Some error");
    }
}
