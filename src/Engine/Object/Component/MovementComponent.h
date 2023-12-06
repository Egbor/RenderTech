#ifndef MOVEMENT_COMPONENT_H
#define MOVEMENT_COMPONENT_H

#include "Engine/Object/Component/EntityComponent.h"

namespace Engine {
	class Actor;
	class CameraComponent;

	CLASSTYPE(MovementComponent)
	class MovementComponent : public EntityComponent {
		GENERATE_BODY(MovementComponent, EntityComponent)

	private:
		Actor* m_linkedActor;
		CameraComponent* m_linkedCamera;

		Vector3 m_moveVector;
		Rotator m_objRotator;
		Rotator m_camRotator;

	public:
		MovementComponent(const ObjectArgument& argument);
		virtual ~MovementComponent() = default;

		void AddMoveVector(const Vector3& vector);
		void AddActorRotator(const Rotator& rotator);
		void AddCameraRotator(const Rotator& rotator);

		void AssociateWithActor(Actor* actor);
		void AssociateWithCamera(CameraComponent* camera);


		void UpdateMovement(Float deltaTime);
	};
}

#endif // !MOVEMENT_COMPONENT_H
