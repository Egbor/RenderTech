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
    }

    void Material::AddTexture(Texture2D* texture) {
        m_textures.push_back(texture);
    }

    void Material::AddShader(Shader* shader) {
        m_shaders[static_cast<Int32>(shader->GetAssociatedStage())] = shader;
    }

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

    Material::Metadata::Metadata() 
        : m_shaderPaths(), m_texturePaths() {

    }

    Material::Metadata* Material::Metadata::SetName(const String& value) {
        return dynamic_cast<Material::Metadata*>(_SetName(value));
    }

    Material::Metadata* Material::Metadata::AddTexturePath(const String& path) {
        m_texturePaths.push_back(path);
        return this;
    }

    Material::Metadata* Material::Metadata::AddShaderPath(const String& path) {
        m_shaderPaths.push_back(path);
        return this;
    }

    IResourceMetadata* Material::Metadata::_SetName(const String& value) {
        m_name = value;
        return this;
    }

    Object* Material::Metadata::Build() {
        assert(!m_name.empty());

        Material* material = ClassType<Material>::CreateObject(ObjectArgument::Dummy());
        
        for (Size i = 0; i < m_texturePaths.size(); i++) {
            material->AddTexture(Core::Load<Texture2D>(m_texturePaths[i]));
        }

        for (Size i = 0; i < m_shaderPaths.size(); i++) {
            material->AddShader(Core::Load<Shader>(m_shaderPaths[i]));
        }

        return material;
    }
}
