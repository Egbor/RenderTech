#ifndef TEXTURE_H
#define TEXTURE_H

#include "Engine/Core/Render/Base/Resource/TextureResource.h"
#include "Engine/Core/System/Resource/ResourceMetadata.h"
#include "Engine/Object/Object.h"

namespace Engine {
    CLASSTYPE(Texture2D)
    class Texture2D : public Object {
        GENERATE_BODY(Texture2D, Object)

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
            Metadata* SetData(TextureResource* resaource);

            bool IsCubemap() const;

            Int32 GetWidth() const;
            Int32 GetHeight() const;

            Object* Build() override;

        private:
            IResourceMetadata* _SetName(const String& value) override;

            String m_name;
            Array<Int8*> m_data;
            Int32 m_width;
            Int32 m_height;
            TextureFormat m_format;
            const TextureType m_type;

            TextureResource* m_resource;
        };

        Texture2D(const ObjectArgument&);
        virtual ~Texture2D();

        bool IsCubemap() const;
        Int32 GetWidth() const;
        Int32 GetHeight() const;
        TextureFormat GetFormat() const;

        TextureResource* GetNativeResource() const;

    private:
        TextureResource* m_nativeTexture;
        bool m_hasExternNativeTexture;
    };
}

#endif // TEXTURE_H
