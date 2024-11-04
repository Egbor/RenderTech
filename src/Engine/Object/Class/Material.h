#ifndef MATERIAL_H
#define MATERIAL_H

#include "Engine/Object/Class/Texture.h"
#include "Engine/Object/Class/Shader.h"

namespace Engine {
    CLASSTYPE(Material)
    class Material : public Object {
        GENERATE_BODY(Material, Object)

        //template<class TObjectClass>
        //friend TObjectClass* LoadResourceFromFile(const String&);

    private:
        Array<Texture2D*> m_textures;
        Array<Shader*> m_shaders;

        //IShaderResourceData* m_shader;

    public:
        Material(const ObjectArgument& argument);
        virtual ~Material();

        void AddTexture(Texture2D* texture);
        void AddShader(Shader* shader);
        //void SetShader(const Array<Int8>& bytecode);

        Array<TextureResource*> GetNativeTextureResources() const;
        Array<ShaderResource*> GetNativeShaderResources() const;
        //IShaderResourceData* GetNativeShaderResource() const;
    };
}

#endif // MATERIAL_H
