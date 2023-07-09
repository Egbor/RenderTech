#ifndef TEXTURE_H
#define TEXTURE_H

#include "Engine/Core/Render/Api/RenderDef.h"
#include "Engine/Object/Object.h"

namespace Engine {
    CLASSTYPE(Texture)
    class Texture : public Object {
        GENERATE_BODY(Texture, Object)

    public:
        Texture(const ObjectArgument& argument);
        Texture(const Texture&) = default;
        virtual ~Texture() = default;

        virtual String GetTag() const;
        virtual TextureFormat GetFormat() const;
    };

    CLASSTYPE(Texture2D)
    class Texture2D : public Texture {
        GENERATE_BODY(Texture2D, Texture)

    public:
        Texture2D(const ObjectArgument& argument);
        Texture2D(const Texture2D&) = default;
        virtual ~Texture2D() = default;

        virtual void Resize(Int32 height, Int32 width, TextureFormat format);

        virtual Int32 GetWidth() const;
        virtual Int32 GetHeight() const;
    };

    CLASSTYPE(CubeTexture2D)
    class CubeTexture2D : public Texture2D {
        GENERATE_BODY(CubeTexture2D, Texture2D)

    private:
        Int32 m_faceId;

    public:
        CubeTexture2D(const ObjectArgument& argument);
        CubeTexture2D(const CubeTexture2D&) = default;
        virtual ~CubeTexture2D() = default;

        void SetFaceId(Int32 id);
        Int32 GetFaceId() const;
    };
}

#endif // TEXTURE_H
