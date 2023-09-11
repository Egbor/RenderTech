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
        Material(const Material&) = default;
        virtual ~Material();

        void SetTexture(TextureSlot slot, Texture2D* texture);
        Texture2D* GetTexture(TextureSlot slot) const;
    };
}

#endif // MATERIAL_H
