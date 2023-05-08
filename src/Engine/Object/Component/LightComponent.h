#ifndef LIGHTCOMPONENT_H
#define LIGHTCOMPONENT_H

#include "Engine/Object/Component/SceneComponent.h"
#include "Engine/Object/Class/Mesh.h"

namespace Engine {
    class LightComponent : public SceneComponent {
        GENERATE_BODY(LightComponent, SceneComponent)

    public:
        Vector4 color;

        LightComponent(const ObjectArgument& argument);
        LightComponent(const LightComponent&) = default;
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
