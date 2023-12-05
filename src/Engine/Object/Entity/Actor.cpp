#include "Engine/Object/Entity/Actor.h"

#include <sstream>
#include <Windows.h>

namespace Engine {
    //GENERATE_RTTI_DEFINITIONS(Actor)
    GENERATE_INSTANTIATION(Actor)

    Actor::Actor(const ObjectArgument& argument)
        : Super(argument) {
        m_inputComponent = ClassType<InputComponent>::CreateObject(ObjectArgument::Dummy());
        m_movementComponent = ClassType<MovementComponent>::CreateObject(ObjectArgument::Dummy());
    }

    Actor::~Actor() {
        DELETE_OBJECT(m_inputComponent)
    }

    void Actor::SetupInputComponent(InputComponent* component) {

    }

    void Actor::OnStart() {
        Super::OnStart();
        SetupInputComponent(m_inputComponent);
    }

    void Actor::OnUpdate(Float deltaTime) {
        Super::OnUpdate(deltaTime);

        m_movementComponent->UpdateMovement(this, deltaTime);
        m_inputComponent->UpdateInputs(1.0f);

        Vector3 position = GetRootComponent()->GetWorldPosition();

        std::stringstream ss;
        ss << "Position: " << position.x << ", " << position.y << ", " << position.z << "(" << deltaTime << ")" << std::endl;
        OutputDebugStringA(ss.str().c_str());

       /* AddEntityLocation(m_movement.Normalize() * deltaTime);*/

        //if (m_componentForRollRotation != nullptr) {
        //    AddEntityRotation(Rotator(0.0f, m_rotation.y * deltaTime, 0.0f));
        //    Float oldRoll = m_componentForRollRotation->GetLocalRotation().x;
        //    Float newRoll = std::clamp(oldRoll + m_rotation.x * deltaTime, -90.0f, 90.0f);
        //    m_componentForRollRotation->SetLocalRotation(Rotator(newRoll, 0.0f, 0.0f));
        //}

        //ResetMovement();
    }

    void Actor::AddMovement(const Vector3& offset) {
        m_movementComponent->AddMoveVector(offset);
    }

    //void Actor::AddPitchRotation(Float value) {
    //    m_rotation.y = 3000.0f * value;
    //}

    //void Actor::AddRollRotation(Float value) {
    //    m_rotation.x = 3000.0f * value;
    //}

    //void Actor::AllowControlCameraRotation(CameraComponent* component) {
    //    m_componentForRollRotation = component;
    //}

    //void Actor::ResetMovement() {
    //    m_movement = Vector3::zero;
    //    m_rotation = Rotator(0.0f, 0.0f, 0.0f);
    //}
}
