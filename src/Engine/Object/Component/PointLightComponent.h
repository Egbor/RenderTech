#ifndef POINTLIGHTCOMPONENT_H
#define POINTLIGHTCOMPONENT_H

#include "Engine/Object/Component/LightComponent.h"

namespace Engine {
    class PointLightComponent : public LightComponent {
        GENERATE_BODY(PointLightComponent, LightComponent)

    private:
        static Mesh* volume;

    public:
        PointLightComponent(const ObjectArgument& argument);
        PointLightComponent(const PointLightComponent&) = default;
        virtual ~PointLightComponent() = default;

        void SetRadius(Float radius);
        Float GetRadius() const;

        Mesh* GetVolume() const override;
    };
}

#endif // POINTLIGHTCOMPONENT_H
