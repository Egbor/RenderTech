#include "Engine/Object/Entity/Actor.h"

#include <algorithm>

namespace Engine {
    GENERATE_RTTI_DEFINITIONS(Actor)

    Actor::Actor(const ObjectArgument& argument)
        : Super(argument), m_componentForRollRotation(GetRootComponent()) {
    }

    void Actor::SetupInputComponent(InputComponent* component) {
        component->ResetBinds();
    }

    void Actor::OnStart() {
        Super::OnStart();
        ResetMovement();
    }

    void Actor::OnUpdate(Float deltaTime) {
        Super::OnUpdate(deltaTime);

        AddEntityLocation(m_movement.Normalize() * deltaTime);
        AddEntityRotation(Rotator(0.0f, m_rotation.y * deltaTime, 0.0f));

        Float oldRoll = m_componentForRollRotation->GetLocalRotation().x;
        Float newRoll = std::clamp(oldRoll + m_rotation.x * deltaTime, -90.0f, 90.0f);
        m_componentForRollRotation->SetLocalRotation(Rotator(newRoll, 0.0f, 0.0f));

        ResetMovement();
    }

    void Actor::AddMovement(const Vector3& offset) {
        m_movement += offset;
    }

    void Actor::AddPitchRotation(Float value) {
        m_rotation.y = 3000.0f * value;
    }

    void Actor::AddRollRotation(Float value) {
        m_rotation.x = 3000.0f * value;
    }

    void Actor::AllowControlCameraRotation(CameraComponent* component) {
        if (component != nullptr) {
            m_componentForRollRotation = component;
        }
    }

    void Actor::ResetMovement() {
        m_movement = Vector3::zero;
        m_rotation = Rotator(0.0f, 0.0f, 0.0f);
    }
}
