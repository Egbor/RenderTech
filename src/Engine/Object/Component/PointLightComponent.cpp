#include "Engine/Object/Component/PointLightComponent.h"
#include "Engine/Core/System/Import/MeshImport.h"

namespace Engine {
    GENERATE_INSTANTIATION(PointLightComponent)

    Mesh* PointLightComponent::volume = nullptr;

    PointLightComponent::PointLightComponent(const ObjectArgument& argument)
        : Super(argument) {
    }

    void PointLightComponent::SetRadius(Float radius) {
        SetLocalScale({ radius, radius, radius });
    }

    Float PointLightComponent::GetRadius() const {
        Vector3 scale = GetLocalScale();
        return (scale.x + scale.y + scale.z) / 3;
    }

    Mesh* PointLightComponent::GetVolume() const {
        if (volume == nullptr) {
            /*MeshImport import("./bin/models/sphere.obj");*/
            volume = nullptr; // import.LoadResource();
        }
        return volume;
    }
}
