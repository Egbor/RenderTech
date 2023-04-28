#ifndef TEXTUREIMPORT_H
#define TEXTUREIMPORT_H

#include "Engine/Core/System/Import/Import.h"
#include "Engine/Object/Class/Texture.h"

namespace Engine {
    class TextureImport : public Import<Texture2D> {
    public:
        TextureImport(const String& filename);
        TextureImport(const TextureImport&) = default;
        virtual ~TextureImport() = default;

        Texture2D* LoadResource() override;
    };

    class CubeTextureImport : public Import<CubeTexture2D> {
    public:
        CubeTextureImport(const String& filename);
        CubeTextureImport(const CubeTextureImport&) = default;
        virtual ~CubeTextureImport() = default;

        CubeTexture2D* LoadResource() override;
        void SaveResource(CubeTexture2D* texture);
    };
}

#endif // TEXTUREIMPORT_H
