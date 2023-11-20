#ifndef SHADERIMPORT_H
#define SHADERIMPORT_H

#include "Engine/Core/System/Import/Import.h"
#include "Engine/Core/Render/Api/Interface/IContext.h"

namespace Engine {
    class ShaderLoader : public AbstractLoader {
    public:
        ShaderLoader(IRenderResourceFactory* factory, ShaderType type, const String& filename);
        virtual ~ShaderLoader() = default;

        IShaderResourceData* LoadShader();

    private:
        void* Load() override;

        IRenderResourceFactory* m_factory;
        ShaderType m_loadType;
    };

    //class ShaderImport : public Import<ShaderDescription> {
    //public:
    //    ShaderImport(const String& filename);
    //    ShaderImport(const ShaderImport&) = default;
    //    virtual ~ShaderImport() = default;

    //    ShaderDescription* LoadResource() override;
    //};
}

#endif // SHADERIMPORT_H
