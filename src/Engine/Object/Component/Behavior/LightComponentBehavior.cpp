#include "Engine/Object/Component/Behavior/LightComponentBehavior.h"

namespace Engine {
    GENERATE_RTTI_DEFINITIONS(LightComponentBehavior)

    LightComponentBehavior::LightComponentBehavior(const ObjectArgument& argument)
        : Super(argument){

    }

    Mesh* LightComponentBehavior::GetCurrentLightVolume() const {
        return (*m_componentsInterator)->GetVolume();
    }

    Matrix4x4 LightComponentBehavior::GetCurrentLightWorldMatrix() const {
        return Matrix4x4::CreateMatrixWorld(
            (*m_componentsInterator)->GetWorldPosition(),
            (*m_componentsInterator)->GetWorldRotation(),
            (*m_componentsInterator)->GetWorldScale()
        );
    }

    Vector4 LightComponentBehavior::GetCurrentValue() const {
        return (*m_componentsInterator)->GetValue();
    }

    Vector4 LightComponentBehavior::GetCurrentColor() const {
        return (*m_componentsInterator)->GetColor();
    }

    void LightComponentBehavior::Init() {
        m_componentsInterator = GetComponents().begin();
    }

    void LightComponentBehavior::ToNext() {
        m_componentsInterator++;
    }

    bool LightComponentBehavior::HasMore() {
        if (m_componentsInterator == GetComponents().end()) {
            return false;
        }
        return true;
    }
}
