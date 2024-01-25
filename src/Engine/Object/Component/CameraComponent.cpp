#include "Engine/Object/Component/CameraComponent.h"
#include "Engine/Rendering/Engine/RenderPass/BaseRenderPass.h"
#include "Engine/Rendering/Engine/RenderPass/LightRenderPass.h"

namespace Engine {
    GENERATE_INSTANTIATION(CameraComponent)

    Matrix4x4 CreateViewMatrix(CameraComponent* component) {
        Vector3 eyePosition = component->GetWorldPosition();
        Vector3 focusPosition = component->GetForward() + eyePosition;
        Vector3 upDirection = component->GetUp();

        return Matrix4x4::CreateMatrixLookAt(eyePosition, focusPosition, upDirection);
    }

    Matrix4x4 CreateProjectionMatrix(CameraComponent* component, Float resolutionWidth, Float resolutionHeight) {
        Float aspectRatio = resolutionWidth / resolutionHeight;

        return Matrix4x4::CreateMatrixPerspective(component->GetFOV(), aspectRatio, component->GetNearZ(), component->GetFarZ());
    }

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
            BaseRenderPass* baseRenderPass = dynamic_cast<BaseRenderPass*>(pass);
            baseRenderPass->SetCamera(CreateViewMatrix(this), CreateProjectionMatrix(this, pass->GetRenderWidth(), pass->GetRenderHeight()));
        } else if(pass->Is(RenderPassType::RP_LIGHT)) {
            LightRenderPass* lightRenderPass = dynamic_cast<LightRenderPass*>(pass);
            lightRenderPass->SetViewProjection(CreateViewMatrix(this), CreateProjectionMatrix(this, pass->GetRenderWidth(), pass->GetRenderHeight()));
        }
    }
}
