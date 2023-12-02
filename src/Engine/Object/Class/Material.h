#ifndef MATERIAL_H
#define MATERIAL_H

#include "Engine/Object/Class/Texture.h"
#include "Engine/Core/Render/Api/Interface/IShaderResource.h"

namespace Engine {
    CLASSTYPE(Material)
    class Material : public Object {
        GENERATE_BODY(Material, Object)

    private:
        Array<Texture2D*> m_textures;
        IShaderResourceData* m_shader;

    public:
        Material(const ObjectArgument& argument);
        virtual ~Material();

        void AddTexture(Texture2D* texture);
        void SetShader(const Array<Int8>& bytecode);

        Array<ITextureResourceData*> GetNativeTextureResources() const;
        IShaderResourceData* GetNativeShaderResource() const;
    };
}

#endif // MATERIAL_H
