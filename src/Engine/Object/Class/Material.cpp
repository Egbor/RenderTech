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

    void Material::Serialize(ISerializer* serializer) {

    }

    void Material::Deserialize(ISerializer* serializer) {
        Super::Deserialize(serializer);

        //ISerializer* subobjectSerializer = nullptr;
        //while (serializer->CreateSubobjectSerializer(&subobjectSerializer)) {
        //    if (subobjectSerializer->GetObjectName() == "Texture2D") {
        //        String path = "";
        //        String map = "";

        //        subobjectSerializer->GetString(argTagPath, path);
        //        subobjectSerializer->GetString(argTagMap, map);

        //        TextureImport importer(path);
        //        if (map == "Albedo") {
        //            SetTexture(TextureSlot::TS_ALBEDO, importer.LoadResource());
        //        } else if (map == "Normal") {
        //            SetTexture(TextureSlot::TS_NORMAL, importer.LoadResource());
        //        } else if (map == "Metallic") {
        //            SetTexture(TextureSlot::TS_METALLIC, importer.LoadResource());
        //        } else if (map == "Roughness") {
        //            SetTexture(TextureSlot::TS_ROUGHNESS, importer.LoadResource());
        //        } else if (map == "AmbinatOcclusion") {
        //            SetTexture(TextureSlot::TS_AO, importer.LoadResource());
        //        }
        //    }
        //}
        //serializer->DispatchSubobjectSerializer(&subobjectSerializer);
    }
}
