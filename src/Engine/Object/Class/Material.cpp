#include "Engine/Object/Class/Material.h"

#include "Engine/Core/System/Import/TextureImport.h"

namespace Engine {
    GENERATE_INSTANTIATION(Material)

    Material::Material(const ObjectArgument& argument)
        : Super(argument), m_textures(5) {
        for (Size i = 0; i < m_textures.size(); i++) {
            m_textures[i] = nullptr;
        }
    }

    Material::~Material() {

    }

    void Material::SetTexture(TextureSlot slot, Texture2D* texture) {
        m_textures[static_cast<Int32>(slot)] = texture;
    }

    Texture2D* Material::GetTexture(TextureSlot slot) const {
        return m_textures[static_cast<Int32>(slot)];
    }
}
