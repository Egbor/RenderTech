#include "Engine/Object/Class/Material.h"
#include "Engine/Core/Core.h"

namespace Engine {
    GENERATE_INSTANTIATION(Material)

    Material::Material(const ObjectArgument& argument)
        : Super(argument), m_textures(), m_shaders(static_cast<Size>(RenderStage::RS_NUM), nullptr) {
    }

    Material::~Material() {
        DELETE_ARRAY_OF_OBJECTS(m_textures);
        DELETE_ARRAY_OF_OBJECTS(m_shaders);
        //DELETE_OBJECT(m_shader);
    }

    void Material::AddTexture(Texture2D* texture) {
        m_textures.push_back(texture);
    }

    void Material::AddShader(Shader* shader) {
        m_shaders[static_cast<Int32>(shader->GetAssociatedStage())] = shader;
    }

    //void Material::SetShader(const Array<Int8>& bytecode) {
    //    IRenderResourceFactory* factory = Core::GetInstance()->GetContext()->QueryResourceFactory();
    //    m_shader = factory->CreateShader(ShaderType::ST_PIXEL, bytecode.size(), bytecode.data());
    //}

    Array<TextureResource*> Material::GetNativeTextureResources() const {
        Array<TextureResource*> nativeTextures(m_textures.size());
        for (Size i = 0; i < nativeTextures.size(); i++) {
            nativeTextures[i] = m_textures[i]->GetNativeResource();
        }
        return nativeTextures;
    }

    Array<ShaderResource*> Material::GetNativeShaderResources() const {
        Array<ShaderResource*> nativeShaders(m_shaders.size());
        for (Size i = 0; i < nativeShaders.size(); i++) {
            nativeShaders[i] = m_shaders[i]->GetNativeResource();
        }
        return nativeShaders;
    }

    //IShaderResourceData* Material::GetNativeShaderResource() const {
    //    return m_shader;
    //}
}
