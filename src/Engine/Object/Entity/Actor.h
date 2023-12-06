#ifndef ACTOR_H
#define ACTOR_H

#include "Engine/Object/Entity/Entity.h"
#include "Engine/Object/Component/CameraComponent.h"
#include "Engine/Object/Component/InputComponent.h"
#include "Engine/Object/Component/MovementComponent.h"

namespace Engine {
    CLASSTYPE(Actor)
    class Actor : public Entity {
        GENERATE_BODY(Actor, Entity)

    private:
        MovementComponent* m_movementComponent;
        InputComponent* m_inputComponent;

    public:
        Actor(const ObjectArgument& argument);
        virtual ~Actor();

        virtual void SetupInputComponent(InputComponent* component);

        void OnStart() override;
        void OnUpdate(Float deltaTime) override;

    protected:
        void AddMovement(const Vector3& offset);
        void AddPitch(Float value);
        void AddRoll(Float value);

        void AllowControlCameraRotation(CameraComponent* component);
    };
}

#endif // ACTOR_H
