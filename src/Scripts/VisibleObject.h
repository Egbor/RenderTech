#ifndef VISIBLEOBJECT_H
#define VISIBLEOBJECT_H

#include "Engine/Object/Entity/Entity.h"
#include "Engine/Object/Component/MeshComponent.h"

namespace Engine {
    CLASSTYPE(VisibleObject)
    class VisibleObject : public Entity {
        GENERATE_BODY(VisibleObject, Entity)

    private:
        MeshComponent* m_meshComponent;

    public:
        VisibleObject(const ObjectArgument& argument);
        VisibleObject(const VisibleObject&) = default;
        virtual ~VisibleObject();

        void OnUpdate(Float deltaTime) override;
    };
}

#endif // VISIBLEOBJECT_H
