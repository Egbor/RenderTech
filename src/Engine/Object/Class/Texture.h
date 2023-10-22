#ifndef TEXTURE_H
#define TEXTURE_H

#include "Engine/Core/Render/Api/Interface/ITextureResource.h"
#include "Engine/Object/Object.h"

namespace Engine {
    CLASSTYPE(Texture)
    class Texture : public Object {
        GENERATE_BODY(Texture, Object)

    public:
        Texture(const ObjectArgument& argument);
        virtual ~Texture() = default;

        Int32 GetWidth() const;
        Int32 GetHeight() const;
        TextureFormat GetFormat() const;

        ITextureResourceData* GetNativeResource() const;

    protected:
        ITextureResourceData* m_nativeTexture;
    };

    CLASSTYPE(Texture2D)
    class Texture2D : public Texture {
        GENERATE_BODY(Texture2D, Texture)

    public:
        Texture2D(const ObjectArgument&);
        virtual ~Texture2D() = default;

        void Create(Int32 width, Int32 height, TextureFormat format, Array<Int8*> rawData);
    };

    CLASSTYPE(TextureCube)
    class TextureCube : public Texture {
        GENERATE_BODY(TextureCube, Texture)

    public:
        TextureCube(const ObjectArgument&);
        virtual ~TextureCube() = default;

        void Create(Int32 width, Int32 height, TextureFormat format, Array<Int8*> rawData);
    };
}

#endif // TEXTURE_H
