#ifndef SPECTATOR_H
#define SPECTATOR_H

#include "Engine/Object/Entity/Actor.h"
#include "Engine/Object/Component/CameraComponent.h"

namespace Engine {
    class Spectator : public Actor {
        GENERATE_BODY(Spectator, Actor)

    private:
        CameraComponent* m_cameraComponent;

    public:
        Spectator(const ObjectArgument& argument);
        Spectator(const Spectator&) = default;
        virtual ~Spectator();

        void SetupInputComponent(InputComponent* component) override;
        void OnUpdate(Float deltaTime) override;

        void OnForwardMovement(Float value);
        void OnRightMovement(Float value);
    };
}

#endif // SPECTATOR_H
