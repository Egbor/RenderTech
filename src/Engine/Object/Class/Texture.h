#ifndef TEXTURE_H
#define TEXTURE_H

#include "Engine/Core/Render/Base/Resource/TextureResource.h"
#include "Engine/Core/System/Resource/ResourceMetadata.h"
#include "Engine/Object/Object.h"

namespace Engine {
    enum class TextureFace {
        TF_POSITIVE_X = 0,
        TF_NEGATIVE_X = 1,
        TF_POSITIVE_Y = 2,
        TF_NEGATIVE_Y = 3,
        TF_POSITIVE_Z = 4,
        TF_NEGATIVE_Z = 5,
        TF_DEFAULT = 0
    };

    CLASSTYPE(Texture)
    class Texture : public Object {
        GENERATE_BODY(Texture, Object)

    public:
        Texture(const ObjectArgument& argument);
        virtual ~Texture() = default;

        Int32 GetWidth() const;
        Int32 GetHeight() const;
        TextureFormat GetFormat() const;

        bool IsCubemap() const;

        TextureResource* GetNativeResource() const;

    protected:
        TextureResource* m_nativeTexture;
    };

    CLASSTYPE(Texture2D)
    class Texture2D : public Texture {
        GENERATE_BODY(Texture2D, Texture)

    public:
        class Metadata : public IResourceMetadata {
        public:
            Metadata(TextureType type);
            ~Metadata();

            Metadata* SetName(const String& value);
            Metadata* SetWidth(Int32 value);
            Metadata* SetHeight(Int32 value);
            Metadata* SetFormat(TextureFormat format);
            Metadata* SetData(const Int8* bits, TextureFace face);

            bool IsCubemap() const;

            Int32 GetWidth() const;
            Int32 GetHeight() const;

            Object* Build() override;

        private:
            String m_name;
            Array<Int8*> m_data;
            Int32 m_width;
            Int32 m_height;
            TextureFormat m_format;
            const TextureType m_type;
        };

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
