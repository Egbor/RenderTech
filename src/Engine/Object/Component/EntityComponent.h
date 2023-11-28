#ifndef ENTITYCOMPONENT_H
#define ENTITYCOMPONENT_H

#include "Engine/Object/Object.h"
#include "Engine/Rendering/Engine/RenderPass/AbstractRenderPass.h"

namespace Engine {
    CLASSTYPE(EntityComponent)
    class EntityComponent : public Object {
        GENERATE_BODY(EntityComponent, Object)

    private:
        HandlerPtr m_register;
        UInt64 m_behaviorID;

    public:
        EntityComponent(const ObjectArgument& argument);
        virtual ~EntityComponent() = default;

        HandlerPtr& GetHandler();

        virtual void CreateRenderState(AbstractRenderPass* pass);

    protected:
        virtual UInt64 GetBehaviorID() const;
    };
}

#endif // ENTITYCOMPONENT_H
