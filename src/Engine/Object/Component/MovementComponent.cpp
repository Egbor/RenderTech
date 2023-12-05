#include "Engine/Object/Component/MovementComponent.h"

namespace Engine {
	GENERATE_INSTANTIATION(MovementComponent);

	MovementComponent::MovementComponent(const ObjectArgument& argument) 
		: EntityComponent(argument), m_moveVector(Vector3::zero) {

	}

	void MovementComponent::AddMoveVector(const Vector3& vector) {
		m_moveVector += vector;
	}

	void MovementComponent::UpdateMovement(Entity* entity, Float deltaTime) {
		if (m_moveVector.Abs() > 1.0f) {
			m_moveVector = m_moveVector.Normalize();
		}
		m_moveVector *= deltaTime;

		entity->AddEntityLocation(m_moveVector);
	}
}