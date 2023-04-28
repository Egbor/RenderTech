#ifndef SHADERDESCRIPTION_H
#define SHADERDESCRIPTION_H

#include "Engine/Rendering/Types.h"

namespace Engine {
    class ShaderDescription {
    private:
        Array<UInt8> m_byteCode;

    public:
        ShaderDescription();
        ShaderDescription(const ShaderDescription&) = default;
        virtual ~ShaderDescription() = default;

        ShaderHandler Create(Size length);
        ShaderInfo GetShaderInfo();
    };
}

#endif // SHADERDESCRIPTION_H
