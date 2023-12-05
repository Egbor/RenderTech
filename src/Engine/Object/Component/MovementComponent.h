#ifndef MOVEMENT_COMPONENT_H
#define MOVEMENT_COMPONENT_H

#include "Engine/Object/Component/EntityComponent.h"
#include "Engine/Object/Entity/Entity.h"

namespace Engine {
	CLASSTYPE(MovementComponent)
	class MovementComponent : public EntityComponent {
		GENERATE_BODY(MovementComponent, EntityComponent)

	private:
		Vector3 m_moveVector;

	public:
		MovementComponent(const ObjectArgument& argument);
		virtual ~MovementComponent() = default;

		void AddMoveVector(const Vector3& vector);
		void UpdateMovement(Entity* entity, Float deltaTime);
	};
}

#endif // !MOVEMENT_COMPONENT_H
