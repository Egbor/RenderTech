#ifndef ENTITYCONTROLLER_H
#define ENTITYCONTROLLER_H

#include "Engine/Object/Entity/Actor.h"

namespace Engine {
    class EntityController : public Object {
        GENERATE_BODY(EntityController, Object)

    private:
        Actor* m_actor;
        InputComponent* m_component;

    public:
        EntityController(const ObjectArgument& arguments);
        virtual ~EntityController() = default;

        void AttachActor(Actor* actor);
        void DetachActor();

        virtual void Execute();
    };
}

#endif // ENTITYCONTROLLER_H
