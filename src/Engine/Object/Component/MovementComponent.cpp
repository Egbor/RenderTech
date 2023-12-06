#include "Engine/Object/Component/MovementComponent.h"
#include "Engine/Object/Entity/Actor.h"

#include <cmath>

namespace Engine {
	GENERATE_INSTANTIATION(MovementComponent);

	MovementComponent::MovementComponent(const ObjectArgument& argument) 
		: EntityComponent(argument), m_moveVector(Vector3::zero), m_objRotator(0.0f, 0.0f, 0.0f)
		, m_linkedActor(nullptr), m_linkedCamera(nullptr) {

	}

	void MovementComponent::AddMoveVector(const Vector3& vector) {
		m_moveVector += vector;
	}

	void MovementComponent::AddActorRotator(const Rotator& rotator) {
		m_objRotator = rotator;
	}

	void MovementComponent::AddCameraRotator(const Rotator& rotator) {
		m_camRotator = rotator;
	}

	void MovementComponent::AssociateWithActor(Actor* actor) {
		m_linkedActor = actor;
	}

	void MovementComponent::AssociateWithCamera(CameraComponent* camera) {
		m_linkedCamera = camera;
	}

	void MovementComponent::UpdateMovement(Float deltaTime) {
		if (m_linkedActor == nullptr) {
			return;
		}
		
		if (m_moveVector.Abs() > 1.0f) {
			m_moveVector = m_moveVector.Normalize();
		}

		m_moveVector *= deltaTime;
		m_linkedActor->AddEntityLocation(m_moveVector);

		if (m_linkedCamera != nullptr) {
			Rotator newObjRotator = m_linkedActor->GetRootComponent()->GetLocalRotation() + m_objRotator * deltaTime;
			Rotator newCamRotator = m_linkedCamera->GetLocalRotation() + m_camRotator * deltaTime;

			newCamRotator.x = std::clamp(newCamRotator.x, -90.0f, 90.0f);

			m_linkedActor->SetEntityRotation(newObjRotator);
			m_linkedCamera->SetLocalRotation(newCamRotator);
		}
	}
}