#include "Engine/Object/Component/CameraComponent.h"
#include "Engine/Rendering/Engine/RenderPass/BaseRenderPass.h"

namespace Engine {
    GENERATE_INSTANTIATION(CameraComponent)

    CameraComponent::CameraComponent(const ObjectArgument& argument)
        : Super(argument), m_fov(defaultFOV), m_far(defaultFar), m_near(defaultNear) {

    }

    Float CameraComponent::GetFOV() const {
        return m_fov;
    }

    Float CameraComponent::GetFarZ() const {
        return m_far;
    }

    Float CameraComponent::GetNearZ() const {
        return m_near;
    }

    UInt64 CameraComponent::GetBehaviorID() const {
        return CameraComponent::TypeIdClass();
    }

    void CameraComponent::CreateRenderState(IRenderPass* pass) {
        if (pass->Is(RenderPassType::RP_BASE)) {
            BaseRenderPass* baseRenderPass = dynamic_cast<BaseRenderPass*>(pass);
        }
    }
}
