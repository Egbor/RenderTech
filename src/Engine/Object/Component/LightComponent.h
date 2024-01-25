#ifndef LIGHTCOMPONENT_H
#define LIGHTCOMPONENT_H

#include "Engine/Object/Component/SceneComponent.h"
#include "Engine/Object/Class/Mesh.h"

namespace Engine {
    CLASSTYPE(LightComponent)
    class LightComponent : public SceneComponent {
        GENERATE_BODY(LightComponent, SceneComponent)

    private:
        PROPERTY(ValueType<Vector4>, color)
        Vector4 m_color;

        PROPERTY(ValueType<Float>, brightness)
        Float m_brightness;

    public:
        LightComponent(const ObjectArgument& argument);
        virtual ~LightComponent() = default;

        void SetColor(const Vector4& rgb);
        void SetBrightness(const Float& brightness);

        Vector4 GetColor() const;
        Float GetBrightness() const;

    protected:
        virtual UInt64 GetBehaviorID() const override;
    };
}

#endif // LIGHTCOMPONENT_H
