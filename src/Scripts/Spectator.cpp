#include "Scripts/Spectator.h"

#include <Windows.h>
#include <algorithm>

namespace Engine {
    GENERATE_RTTI_DEFINITIONS(Spectator)

    Spectator::Spectator(const ObjectArgument& argument)
        : Super(argument) {
        m_cameraComponent = CreateDefaultSubobject<CameraComponent>("CameraComponent");
        m_cameraComponent->AttachToComponent(GetRootComponent());

        AllowControlCameraRotation(m_cameraComponent);
    }

    Spectator::~Spectator() {
        DELETE_OBJECT(m_cameraComponent);
    }

    void Spectator::SetupInputComponent(InputComponent* component) {
        Super::SetupInputComponent(component);
        component->BindAxis("Forword", this, &Spectator::OnForwardMovement);
        component->BindAxis("Right", this, &Spectator::OnRightMovement);
        component->BindAxis("LookRight", this, &Spectator::AddPitchRotation);
        component->BindAxis("LookUp", this, &Spectator::AddRollRotation);
    }

    void Spectator::OnUpdate(Float deltaTime) {
        Super::OnUpdate(deltaTime);
    }

    void Spectator::OnForwardMovement(Float value) {
       AddMovement(m_cameraComponent->GetForward() * value);
    }

    void Spectator::OnRightMovement(Float value) {
       AddMovement(GetEntityRight() * value);
    }
}
