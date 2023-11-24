#ifndef SHADERIMPORT_H
#define SHADERIMPORT_H

#include "Engine/Core/Render/Api/Interface/IShaderResource.h"

namespace Engine {
    class ShaderLoader {
    public:
        static IShaderResourceData* Load(const String& filename, ShaderType type);
    };
}

#endif // SHADERIMPORT_H
