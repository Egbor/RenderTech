#ifndef WORLDMODE_H
#define WORLDMODE_H

#include "Engine/Object/Object.h"

namespace Engine {
    CLASSTYPE(WorldMode)
    class WorldMode : public Object {
        GENERATE_BODY(WorldMode, Object)

    private:
        IClass* m_actorClass;
        IClass* m_controllerClass;

    public:
        WorldMode(const ObjectArgument& argument);
        virtual ~WorldMode() = default;

        void SetActor(IClass* actorClass);
        void SetController(IClass* controllerClass);

        IClass* GetActorClass() const;
        IClass* GetControllerClass() const;
    };
}

#endif // WORLDMODE_H
