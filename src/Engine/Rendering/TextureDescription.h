#ifndef TEXTUREDESCRIPTION_H
#define TEXTUREDESCRIPTION_H

#include "Engine/Rendering/Types.h"

namespace Engine {
    class TextureDescription {
    private:
        Int32 m_width;
        Int32 m_height;
        TextureFormat m_format;
        Array<UInt8> m_data;

    public:
        TextureDescription();
        TextureDescription(const TextureDescription&) = default;
        virtual ~TextureDescription() = default;

        TextureHandler Create(Int32 width, Int32 height, TextureFormat format, const void* data);
        TextureInfo GetTextureInfo();
    };
}

#endif // TEXTUREDESCRIPTION_H
