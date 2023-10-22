#ifndef MATERIAL_H
#define MATERIAL_H

#include "Engine/Object/Class/Texture.h"
#include "Engine/Core/Render/Api/RenderDef.h"

namespace Engine {
    CLASSTYPE(Material)
    class Material : public Object {
        GENERATE_BODY(Material, Object)

    private:
        Array<Texture2D*> m_textures;

    public:
        Material(const ObjectArgument& argument);
        virtual ~Material() = default;

        void SetTexture(TextureSlot slot, Texture2D* texture);
        Texture2D* GetTexture(TextureSlot slot) const;

        Array<ITextureResourceData*> GetNativeTextureResources() const;
        IShaderResourceData* GetNativeShaderResource() const;
    };
}

#endif // MATERIAL_H
