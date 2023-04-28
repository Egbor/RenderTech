#include "Engine/Object/Component/Behavior/CameraComponentBehavior.h"

namespace Engine {
    GENERATE_RTTI_DEFINITIONS(CameraComponentBehavior)

    CameraComponentBehavior::CameraComponentBehavior(const ObjectArgument& argument)
        : Super(argument) {

    }

    Matrix4x4 CameraComponentBehavior::GetCameraComponentViewMatrix() {
        CameraComponent* component = *GetComponents().begin();

        Vector3 eyePosition = component->GetWorldPosition();
        Vector3 focusPosition = component->GetForward() + eyePosition;
        Vector3 upDirection = component->GetUp();

        return Matrix4x4::CreateMatrixLookAt(eyePosition, focusPosition, upDirection);
    }

    Matrix4x4 CameraComponentBehavior::GetCameraComponentPerspectiveMatrix(Float aspectRatio) {
        CameraComponent* component = *GetComponents().begin();
        return Matrix4x4::CreateMatrixPerspective(
            component->GetFOV(),
            aspectRatio,
            component->GetNearZ(),
            component->GetFarZ()
        );
    }
}
