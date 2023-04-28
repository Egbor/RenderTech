#ifndef WORLDMODE_H
#define WORLDMODE_H

#include "Engine/Object/Object.h"

namespace Engine {
    class WorldMode : public Object {
        GENERATE_BODY(WorldMode, Object)

    private:
        MetaClass* m_actorClass;
        MetaClass* m_controllerClass;

    public:
        WorldMode(const ObjectArgument& argument);
        virtual ~WorldMode() = default;

        void SetActor(MetaClass* actorClass);
        void SetController(MetaClass* controllerClass);

        MetaClass* GetActorClass() const;
        MetaClass* GetControllerClass() const;
    };
}

#endif // WORLDMODE_H
