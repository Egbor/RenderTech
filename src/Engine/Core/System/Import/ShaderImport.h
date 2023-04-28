#ifndef SHADERIMPORT_H
#define SHADERIMPORT_H

#include "Engine/Core/System/Import/Import.h"
#include "Engine/Rendering/ShaderDescription.h"

namespace Engine {
    class ShaderImport : public Import<ShaderDescription> {
    public:
        ShaderImport(const String& filename);
        ShaderImport(const ShaderImport&) = default;
        virtual ~ShaderImport() = default;

        ShaderDescription* LoadResource() override;
    };
}

#endif // SHADERIMPORT_H
