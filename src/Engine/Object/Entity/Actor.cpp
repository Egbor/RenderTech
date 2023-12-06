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

        m_movementComponent->AssociateWithActor(this);
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

        m_movementComponent->UpdateMovement(deltaTime);
        m_inputComponent->UpdateInputs();

        //Vector3 position = GetRootComponent()->GetWorldPosition();

        //std::stringstream ss;
        //ss << "Position: " << position.x << ", " << position.y << ", " << position.z << "(" << deltaTime << ")" << std::endl;
        //OutputDebugStringA(ss.str().c_str());
    }

    void Actor::AddMovement(const Vector3& offset) {
        m_movementComponent->AddMoveVector(offset);
    }

    void Actor::AddPitch(Float value) {
        m_movementComponent->AddActorRotator(Rotator(0.0f, value * 15.0f, 0.0f));
    }

    void Actor::AddRoll(Float value) {
        m_movementComponent->AddCameraRotator(Rotator(value * 15.0f, 0.0f, 0.0f));
    }

    void Actor::AllowControlCameraRotation(CameraComponent* component) {
        m_movementComponent->AssociateWithCamera(component);
    }
}
