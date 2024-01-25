#include "Engine/Object/Component/LightComponent.h"
#include "Engine/Core/System/Exception/EngineException.h"

namespace Engine {
    GENERATE_INSTANTIATION(LightComponent)

    LightComponent::LightComponent(const ObjectArgument& argument)
        : Super(argument), m_color(1.0f, 1.0f, 1.0f, 1.0f), m_brightness(1.0f) {

    }

    void LightComponent::SetColor(const Vector4& rgb) {
        m_color = rgb;
    }

    void LightComponent::SetBrightness(const Float& brightness) {
        m_brightness = brightness;
    }

    Vector4 LightComponent::GetColor() const {
        return m_color;
    }

    Float LightComponent::GetBrightness() const {
        return m_brightness;
    }

    UInt64 LightComponent::GetBehaviorID() const {
        return LightComponent::TypeIdClass();
    }
}
