#include "Engine/Object/Class/Material.h"
#include "Engine/Core/Core.h"

namespace Engine {
    GENERATE_INSTANTIATION(Material)

    Material::Material(const ObjectArgument& argument)
        : Super(argument), m_textures(), m_shader(nullptr) {
    }

    Material::~Material() {
        DELETE_ARRAY_OF_OBJECTS(m_textures);
        DELETE_OBJECT(m_shader);
    }

    void Material::AddTexture(Texture2D* texture) {
        m_textures.push_back(texture);
    }

    void Material::SetShader(const Array<Int8>& bytecode) {
        IRenderResourceFactory* factory = Core::GetInstance()->GetContext()->QueryResourceFactory();
        m_shader = factory->CreateShader(ShaderType::ST_PIXEL, bytecode.size(), bytecode.data());
    }

    Array<ITextureResourceData*> Material::GetNativeTextureResources() const {
        Array<ITextureResourceData*> nativeTextures(m_textures.size());
        for (Size i = 0; i < nativeTextures.size(); i++) {
            nativeTextures[i] = m_textures[i]->GetNativeResource();
        }
        return nativeTextures;
    }

    IShaderResourceData* Material::GetNativeShaderResource() const {
        return m_shader;
    }
}
