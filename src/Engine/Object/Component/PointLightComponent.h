#ifndef POINTLIGHTCOMPONENT_H
#define POINTLIGHTCOMPONENT_H

#include "Engine/Object/Component/LightComponent.h"

namespace Engine {
    CLASSTYPE(PointLightComponent)
    class PointLightComponent : public LightComponent {
        GENERATE_BODY(PointLightComponent, LightComponent)

    public:
        PointLightComponent(const ObjectArgument& argument);
        virtual ~PointLightComponent() = default;

        void SetRadius(Float radius);
        Float GetRadius() const;

        void CreateRenderState(AbstractRenderPass* pass) override;

    private:
        Float m_radius;
    };
}

#endif // POINTLIGHTCOMPONENT_H
