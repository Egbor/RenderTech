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
        //Rotator m_rotation;

        //SceneComponent* m_componentForRollRotation;

        MovementComponent* m_movementComponent;
        InputComponent* m_inputComponent;

    public:
        Actor(const ObjectArgument& argument);
        virtual ~Actor();

        virtual void SetupInputComponent(InputComponent* component);

        void OnStart() override;
        void OnUpdate(Float deltaTime) override;

        void AddMovement(const Vector3& offset);
    //    void AddPitchRotation(Float value);
    //    void AddRollRotation(Float value);

    //protected:
    //    void AllowControlCameraRotation(CameraComponent* component);

    //private:
    //    void ResetMovement();
    };
}

#endif // ACTOR_H
