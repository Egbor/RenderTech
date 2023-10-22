#include "Engine/Object/Class/Texture.h"
#include "Engine/Object/Global/EngineConfig.h"

namespace Engine {
    GENERATE_INSTANTIATION(Texture)

    Texture::Texture(const ObjectArgument& argument)
        : Super(argument), m_nativeTexture(nullptr) {
    }

    TextureFormat Texture::GetFormat() const {
        return m_nativeTexture->GetFormat();
    }

    Int32 Texture::GetWidth() const {
        return m_nativeTexture->GetWidth();
    }

    Int32 Texture::GetHeight() const {
        return m_nativeTexture->GetHeight();
    }

    ITextureResourceData* Texture::GetNativeResource() const {
        return m_nativeTexture;
    }

    GENERATE_INSTANTIATION(Texture2D)

    Texture2D::Texture2D(const ObjectArgument& argument)
        : Super(argument) {

    }

    void Texture2D::Create(Int32 width, Int32 height, TextureFormat format, Array<Int8*> rawData) {
        IRenderResourceFactory* factory = EngineConfig::GetInstance().GetContext()->QueryResourceFactory();
        m_nativeTexture = factory->CreateTexture(TextureType::TT_DEFAULT, format, width, height, rawData);
    }

    GENERATE_INSTANTIATION(TextureCube)

    TextureCube::TextureCube(const ObjectArgument& argument)
        : Super(argument) {

    }

    void TextureCube::Create(Int32 width, Int32 height, TextureFormat format, Array<Int8*> rawData) {
        IRenderResourceFactory* factory = EngineConfig::GetInstance().GetContext()->QueryResourceFactory();
        m_nativeTexture = factory->CreateTexture(TextureType::TT_CUBE, format, width, height, rawData);
    }
}
