#include "Engine/Object/Class/Texture.h"
#include "Engine/Core/Core.h"

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

    bool Texture::IsCubemap() const {
        return false;
    }

    TextureResource* Texture::GetNativeResource() const {
        return m_nativeTexture;
    }

    GENERATE_INSTANTIATION(Texture2D)

    Texture2D::Texture2D(const ObjectArgument& argument)
        : Super(argument) {

    }

    void Texture2D::Create(Int32 width, Int32 height, TextureFormat format, Array<Int8*> rawData) {
        assert(rawData.size() == 1);

        IRenderResourceFactory* factory = Core::GetInstance()->GetContext()->QueryResourceFactory();
        m_nativeTexture = factory->CreateTexture(TextureType::TT_DEFAULT, format, width, height, rawData);
    }

    Texture2D::Metadata::Metadata(TextureType type) 
        : m_type(type), m_data(IsCubemap() ? NUMBER_OF_FACES_FOR_CUBEMAP_TEXTURE : NUMBER_OF_FACES_FOR_SINGLE_TEXTURE)
        , m_width(0), m_height(0) {
        
    }

    Texture2D::Metadata* Texture2D::Metadata::SetSize(Int32 width, Int32 height) {
        assert(!IsCubemap() || (width == height));

        m_width = width;
        m_height = height;

        return this;
    }

    Texture2D::Metadata* Texture2D::Metadata::SetFormat(TextureFormat format) {
        m_format = format;
    }

    Int8** Texture2D::Metadata::GetData(TextureFace face) {
        Int32 faceIndex = static_cast<Int32>(face);

        assert(IsCubemap() || (faceIndex == 0));

        return &m_data[faceIndex];
    }

    Object* Texture2D::Metadata::Build() {
        IRenderResourceFactory* factory = Core::GetInstance()->GetContext()->QueryResourceFactory();

        Texture2D* texture = ClassType<Texture2D>::CreateObject(ObjectArgument::Dummy());
        texture->m_nativeTexture = factory->CreateTexture(m_type, m_format, m_width, m_height, m_data);

        return texture;
    }

    bool Texture2D::Metadata::IsCubemap() const {
        return m_type == TextureType::TT_CUBE || m_type == TextureType::TT_DEPTH_CUBE;
    }

    GENERATE_INSTANTIATION(TextureCube)

    TextureCube::TextureCube(const ObjectArgument& argument)
        : Super(argument) {

    }

    void TextureCube::Create(Int32 width, Int32 height, TextureFormat format, Array<Int8*> rawData) {
        assert(rawData.size() == 6);

        IRenderResourceFactory* factory = Core::GetInstance()->GetContext()->QueryResourceFactory();
        m_nativeTexture = factory->CreateTexture(TextureType::TT_CUBE, format, width, height, rawData);
    }
}
