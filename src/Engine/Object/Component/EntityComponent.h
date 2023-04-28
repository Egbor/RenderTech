#ifndef ENTITYCOMPONENT_H
#define ENTITYCOMPONENT_H

#include "Engine/Object/Object.h"

namespace Engine {
    class EntityComponent : public Object {
        GENERATE_BODY(EntityComponent, Object)

    private:
        HandlerPtr m_register;
        UInt64 m_behaviorID;

    public:
        EntityComponent(const ObjectArgument& argument);
        EntityComponent(const EntityComponent&) = default;
        virtual ~EntityComponent() = default;

        HandlerPtr& GetHandler();

    protected:
        void Register();
        void UnRegister();

        virtual UInt64 GetBehaviorID() const;
    };
}

#endif // ENTITYCOMPONENT_H
