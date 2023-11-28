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

    void CameraComponent::CreateRenderState(AbstractRenderPass* pass) {
        if (pass->Is(RenderPassType::RP_BASE)) {
            Vector3 eyePosition = GetWorldPosition();
            Vector3 focusPosition = GetForward() + eyePosition;
            Vector3 upDirection = GetUp();
            Float aspectRatio = pass->GetRenderWidth() / pass->GetRenderHeight();

            Matrix4x4 view = Matrix4x4::CreateMatrixLookAt(eyePosition, focusPosition, upDirection);
            Matrix4x4 proj = Matrix4x4::CreateMatrixPerspective(m_fov, aspectRatio, m_near, m_far);

            BaseRenderPass* baseRenderPass = dynamic_cast<BaseRenderPass*>(pass);
            baseRenderPass->SetViewProjection(view * proj);
        }
    }
}
