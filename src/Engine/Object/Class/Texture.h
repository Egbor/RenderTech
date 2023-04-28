#ifndef TEXTURE_H
#define TEXTURE_H

#include "Engine/Core/Render/Api/RenderDef.h"
#include "Engine/Object/Object.h"

namespace Engine {
    class Texture : public Object {
        GENERATE_BODY(Texture, Object)

    public:
        Texture(const ObjectArgument& argument);
        Texture(const Texture&) = default;
        virtual ~Texture() = default;

        virtual String GetTag() const;
        virtual TextureFormat GetFormat() const;
    };

    class Texture2D : public Texture {
        GENERATE_BODY(Texture2D, Texture)

    public:
        Texture2D(const ObjectArgument& argument);
        Texture2D(const Texture2D&) = default;
        virtual ~Texture2D() = default;

        virtual Int32 GetWidth() const;
        virtual Int32 GetHeight() const;
    };

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
