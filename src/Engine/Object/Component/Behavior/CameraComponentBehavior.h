#ifndef CAMERACOMPONENTBEHAVIOR_H
#define CAMERACOMPONENTBEHAVIOR_H

#include "Engine/Object/Component/Behavior/EntityComponentBehavior.h"
#include "Engine/Object/Component/CameraComponent.h"

#include "Engine/Math/Matrix.h"

namespace Engine {
    CLASSTYPE(CameraComponentBehavior)
    class CameraComponentBehavior : public EntityComponentBehavior<CameraComponent> {
        GENERATE_BODY(CameraComponentBehavior, EntityComponentBehavior<CameraComponent>)

    public:
        CameraComponentBehavior(const ObjectArgument& argument);
        CameraComponentBehavior(const CameraComponentBehavior&) = default;
        virtual ~CameraComponentBehavior() = default;

        Matrix4x4 GetCameraComponentViewMatrix();
        Matrix4x4 GetCameraComponentPerspectiveMatrix(Float aspectRatio);
    };
}

#endif // CAMERACOMPONENTBEHAVIOR_H
