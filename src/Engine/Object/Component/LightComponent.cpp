#include "Engine/Object/Component/LightComponent.h"
#include "Engine/Core/System/Exception/EngineException.h"

namespace Engine {
    GENERATE_INSTANTIATION(LightComponent)

    LightComponent::LightComponent(const ObjectArgument& argument)
        : Super(argument), m_color(1.0f, 1.0f, 1.0f, 1.0f) {

    }

    void LightComponent::SetColor(const Vector4& rgb) {
        m_color = rgb;
    }

    Vector4 LightComponent::GetColor() const {
        return m_color;
    }

    Vector4 LightComponent::GetValue() const {
        Vector3 vec = GetWorldPosition();
        return Vector4(vec.x, vec.y, vec.z, 1.0f);
    }

    Mesh* LightComponent::GetVolume() const {
       throw new EngineException("[LightComponent] LightComponent::GetVolume() is abstract method without any features");
    }

    UInt64 LightComponent::GetBehaviorID() const {
        return LightComponent::TypeIdClass();
    }
}
