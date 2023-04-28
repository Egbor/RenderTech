#ifndef LIGHTCOMPONENTBEHAVIOR_H
#define LIGHTCOMPONENTBEHAVIOR_H

#include "Engine/Object/Component/Behavior/EntityComponentBehavior.h"
#include "Engine/Object/Component/LightComponent.h"

#include "Engine/Math/Matrix.h"

namespace Engine {
    class LightComponentBehavior : public EntityComponentBehavior<LightComponent> {
        GENERATE_BODY(LightComponentBehavior, EntityComponentBehavior<LightComponent>)

    private:
        ConstListIterator<LightComponent*> m_componentsInterator;

    public:
        LightComponentBehavior(const ObjectArgument& argument);
        LightComponentBehavior(const LightComponentBehavior&) = default;
        virtual ~LightComponentBehavior() = default;

        Mesh* GetCurrentLightVolume() const;
        Matrix4x4 GetCurrentLightWorldMatrix() const;

        Vector4 GetCurrentValue() const;
        Vector4 GetCurrentColor() const;

        void Init();
        void ToNext();
        bool HasMore();
    };
}

#endif // LIGHTCOMPONENTBEHAVIOR_H
