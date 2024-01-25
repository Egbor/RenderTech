#include "Engine/Object/Component/PointLightComponent.h"
#include "Engine/Core/System/Resource/Resource.h"
#include "Engine/Rendering/Engine/RenderPass/LightRenderPass.h"

namespace Engine {
    GENERATE_INSTANTIATION(PointLightComponent)

    PointLightComponent::PointLightComponent(const ObjectArgument& argument)
        : Super(argument), m_radius(1.0f) {
    }

    void PointLightComponent::SetRadius(Float radius) {
        m_radius = radius;
    }

    Float PointLightComponent::GetRadius() const {
        return m_radius;
    }

    void PointLightComponent::CreateRenderState(AbstractRenderPass* pass) {
        static Mesh* pointLightSpace = Resource::Load<Mesh*>("assets/models/Sphere.fbx");
        
        if (pass->Is(RenderPassType::RP_LIGHT)) {
            LightRenderPass* lightRenderPass = dynamic_cast<LightRenderPass*>(pass);
            RenderLight& light = lightRenderPass->ReserveLightInQueue(LightType::LT_POINT);

            Vector3 scale = Vector3(m_radius, m_radius, m_radius);
            Vector3 position = GetWorldPosition();
            Rotator rotation = GetWorldRotation();

            light.color = GetColor();
            light.brightness = GetBrightness();
            light.value = Vector4(position.x, position.y, position.z, 1.0f);
            light.space.world = Matrix4x4::CreateMatrixWorld(position, rotation, scale);
            light.space.mesh = pointLightSpace;
        }
    }
}
