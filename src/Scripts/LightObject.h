#ifndef LIGHTOBJECT_H
#define LIGHTOBJECT_H

#include "Engine/Object/Entity/Entity.h"
#include "Engine/Object/Component/PointLightComponent.h"

namespace Engine {
    CLASSTYPE(LightObject)
    class LightObject : public Entity {
        GENERATE_BODY(LightObject, Entity)

    private:
        PointLightComponent* m_lightComponent1;
        PointLightComponent* m_lightComponent2;
        PointLightComponent* m_lightComponent3;
        PointLightComponent* m_lightComponent4;

    public:
        LightObject(const ObjectArgument& argument);
        LightObject(const LightObject&) = default;
        virtual ~LightObject();

        void OnUpdate(Float deltaTime) override;
    };
}

#endif // LIGHTOBJECT_H
