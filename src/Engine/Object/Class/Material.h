#ifndef MATERIAL_H
#define MATERIAL_H

#include "Engine/Object/Class/Texture.h"
#include "Engine/Object/Class/Shader.h"

namespace Engine {
    CLASSTYPE(Material)
    class Material : public Object {
        GENERATE_BODY(Material, Object)

    private:
        Array<Texture2D*> m_textures;
        Array<Shader*> m_shaders;

    public:
        class Metadata : public IResourceMetadata {
        public:
            Metadata();
            ~Metadata() = default;

            Metadata* SetName(const String& value);
            Metadata* AddTexturePath(const String& path);
            Metadata* AddShaderPath(const String& path);

            Object* Build() override;

        private:
            IResourceMetadata* _SetName(const String& value) override;

            String m_name;
            Array<String> m_texturePaths;
            Array<String> m_shaderPaths;
        };

        Material(const ObjectArgument& argument);
        virtual ~Material();

        void AddTexture(Texture2D* texture);
        void AddShader(Shader* shader);

        Array<TextureResource*> GetNativeTextureResources() const;
        Array<ShaderResource*> GetNativeShaderResources() const;
    };
}

#endif // MATERIAL_H
