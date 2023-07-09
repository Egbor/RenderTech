#ifndef WORLDMODE_H
#define WORLDMODE_H

#include "Engine/Object/Object.h"

namespace Engine {
    CLASSTYPE(WorldMode)
    class WorldMode : public Object {
        GENERATE_BODY(WorldMode, Object)

    private:
        ObjectType* m_actorClass;
        ObjectType* m_controllerClass;

    public:
        WorldMode(const ObjectArgument& argument);
        virtual ~WorldMode() = default;

        void SetActor(ObjectType* actorClass);
        void SetController(ObjectType* controllerClass);

        ObjectType* GetActorClass() const;
        ObjectType* GetControllerClass() const;
    };
}

#endif // WORLDMODE_H
