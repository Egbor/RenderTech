#include "Engine/Object/Class/Texture.h"
#include "Engine/Core/Core.h"

namespace Engine {
    //GENERATE_INSTANTIATION(Texture)

    //Texture::Texture(const ObjectArgument& argument)
    //    : Super(argument), m_nativeTexture(nullptr) {
    //}

    GENERATE_INSTANTIATION(Texture2D)

    Texture2D::Texture2D(const ObjectArgument& argument)
        : Super(argument), m_nativeTexture(nullptr), m_hasExternNativeTexture(false) {

    }

    Texture2D::~Texture2D() {
        if (!m_hasExternNativeTexture) {
            DELETE_OBJECT(m_nativeTexture);
        }
    }

    TextureFormat Texture2D::GetFormat() const {
        return m_nativeTexture->GetFormat();
    }

    Int32 Texture2D::GetWidth() const {
        return m_nativeTexture->GetWidth();
    }

    Int32 Texture2D::GetHeight() const {
        return m_nativeTexture->GetHeight();
    }

    bool Texture2D::IsCubemap() const {
        return m_nativeTexture->IsCubemap();
    }

    TextureResource* Texture2D::GetNativeResource() const {
        return m_nativeTexture;
    }

    //void Texture2D::Create(Int32 width, Int32 height, TextureFormat format, Array<Int8*> rawData) {
    //    assert(rawData.size() == 1);

    //    IRenderResourceFactory* factory = Core::GetInstance()->GetContext()->QueryResourceFactory();
    //    m_nativeTexture = factory->CreateTexture(TextureType::TT_DEFAULT, format, width, height, rawData);
    //}

    Texture2D::Metadata::Metadata(TextureType type) 
        : m_type(type), m_data(IsCubemap() ? NUMBER_OF_FACES_FOR_CUBEMAP_TEXTURE : NUMBER_OF_FACES_FOR_SINGLE_TEXTURE)
        , m_width(0), m_height(0), m_name(), m_resource(nullptr) {
        
    }

    Texture2D::Metadata::~Metadata() {
        for (Size i = 0; i < m_data.size(); i++) {
            delete[] m_data[i];
            m_data[i] = nullptr;
        }
    }

    Texture2D::Metadata* Texture2D::Metadata::SetName(const String& value) {
        return dynamic_cast<Texture2D::Metadata*>(_SetName(value));
    }

    Texture2D::Metadata* Texture2D::Metadata::SetWidth(Int32 value) {
        m_width = value;
        return this;
    }

    Texture2D::Metadata* Texture2D::Metadata::SetHeight(Int32 value) {
        m_height = value;
        return this;
    }

    Texture2D::Metadata* Texture2D::Metadata::SetFormat(TextureFormat format) {
        m_format = format;
        return this;
    }

    Texture2D::Metadata* Texture2D::Metadata::SetData(const Int8* bits, TextureFace face) {
        Int32 faceIndex = static_cast<Int32>(face);
        Int32 faceSize = m_height * m_width * GetBytesPrePixel(m_format);

        assert(faceIndex < m_data.size());

        if (m_data[faceIndex] != nullptr) {
            delete[] m_data[faceIndex];
            m_data[faceIndex] = nullptr;
        }

        m_data[faceIndex] = new Int8[faceSize];
        memcpy_s(m_data[faceIndex], faceSize, bits, faceSize);

        return this;
    }

    Texture2D::Metadata* Texture2D::Metadata::SetData(TextureResource* resaource) {
        m_resource = resaource;
        return this;
    }

    IResourceMetadata* Texture2D::Metadata::_SetName(const String& name) {
        m_name = value;
        return this;
    }

    Object* Texture2D::Metadata::Build() {
        Texture2D* texture = ClassType<Texture2D>::CreateObject(ObjectArgument::Dummy());

        assert(!m_name.empty());

        if (m_resource == nullptr) {
            IRenderResourceFactory* factory = Core::GetInstance()->GetContext()->QueryResourceFactory();
            texture->m_nativeTexture = factory->CreateTexture(m_type, m_name + "_tex2dRes", m_format, m_width, m_height, m_data);
        } else {
            texture->m_nativeTexture = m_resource;
            texture->m_hasExternNativeTexture = true;
        }

        texture->SetName(m_name);
        return texture;
    }

    bool Texture2D::Metadata::IsCubemap() const {
        return m_type == TextureType::TT_CUBE || m_type == TextureType::TT_DEPTH_CUBE;
    }

    Int32 Texture2D::Metadata::GetWidth() const {
        return m_width;
    }

    Int32 Texture2D::Metadata::GetHeight() const {
        return m_height;
    }

    //GENERATE_INSTANTIATION(TextureCube)

    //TextureCube::TextureCube(const ObjectArgument& argument)
    //    : Super(argument) {

    //}

    //void TextureCube::Create(Int32 width, Int32 height, TextureFormat format, Array<Int8*> rawData) {
    //    assert(rawData.size() == 6);

    //    IRenderResourceFactory* factory = Core::GetInstance()->GetContext()->QueryResourceFactory();
    //    m_nativeTexture = factory->CreateTexture(TextureType::TT_CUBE, format, width, height, rawData);
    //}
}
