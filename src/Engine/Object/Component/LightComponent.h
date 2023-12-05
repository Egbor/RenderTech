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

    public:
        LightComponent(const ObjectArgument& argument);
        virtual ~LightComponent() = default;

        void SetColor(const Vector4& rgb);
        Vector4 GetColor() const;

        virtual Mesh* GetVolume() const;
        virtual Vector4 GetValue() const;

    protected:
        virtual UInt64 GetBehaviorID() const override;
    };
}

#endif // LIGHTCOMPONENT_H
