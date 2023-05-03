#include "Engine/Object/Class/Texture.h"
#include "Engine/Core/System/Exception/EngineException.h"

#include <algorithm>

namespace Engine {
    GENERATE_RTTI_DEFINITIONS(Texture)

    Texture::Texture(const ObjectArgument& argument)
        : Super(argument) {
    }

    String Texture::GetTag() const {
        throw EngineException("[Texture] Texture::GetTag() is abstract method without any features");
    }

    TextureFormat Texture::GetFormat() const {
        throw EngineException("[Texture] Texture::GetFormat() is abstract method without any features");
    }

    GENERATE_RTTI_DEFINITIONS(Texture2D)

    Texture2D::Texture2D(const ObjectArgument& argument)
        : Super(argument) {

    }

    void Texture2D::Resize(Int32 height, Int32 width, TextureFormat format) {
        throw EngineException("[Texture2D] Texture2D::Resize() is abstract method without any features");
    }

    Int32 Texture2D::GetWidth() const {
        throw EngineException("[Texture2D] Texture2D::GetWidth() is abstract method without any features");
    }

    Int32 Texture2D::GetHeight() const {
        throw EngineException("[Texture2D] Texture2D::GetHeight() is abstract method without any features");
    }

    GENERATE_RTTI_DEFINITIONS(CubeTexture2D)

    CubeTexture2D::CubeTexture2D(const ObjectArgument& argument)
        : Super(argument), m_faceId(0) {

    }

    void CubeTexture2D::SetFaceId(Int32 id) {
        m_faceId = std::clamp(id, 0, 5);
    }

    Int32 CubeTexture2D::GetFaceId() const {
        return m_faceId;
    }
}
