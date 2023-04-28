#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H

#include "Engine/Object/Component/SceneComponent.h"

namespace Engine {
    class CameraComponent : public SceneComponent {
        GENERATE_BODY(CameraComponent, SceneComponent)

    private:
        Float m_fov;
        Float m_far;
        Float m_near;

    public:
        CameraComponent(const ObjectArgument& argument);
        CameraComponent(const CameraComponent&) = default;
        virtual ~CameraComponent() = default;

        Float GetFOV() const;
        Float GetFarZ() const;
        Float GetNearZ() const;

    protected:
        virtual UInt64 GetBehaviorID() const override;

    private:
        static constexpr Float defaultFOV = 60.0f * RADIAN_COEFFICIENT;
        static constexpr Float defaultFar = 1000.0f;
        static constexpr Float defaultNear = 0.1f;
    };
}

#endif // CAMERACOMPONENT_H
