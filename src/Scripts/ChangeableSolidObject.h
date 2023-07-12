#ifndef CHANGEABLESOLIDOBJECT
#define CHANGEABLESOLIDOBJECT

#include "Engine/Object/Entity/Actor.h"
#include "Engine/Object/Component/MeshComponent.h"

namespace Engine {
	CLASSTYPE(ChangeableSolidObject)
	class ChangeableSolidObject : public Actor {
		GENERATE_BODY(ChangeableSolidObject, Actor)

	private:
		PROPERTY(ClassType<MeshComponent>, meshComponent)
		MeshComponent* m_meshComponent;

	public:
		ChangeableSolidObject(const ObjectArgument& arguments);
		virtual ~ChangeableSolidObject() = default;
	};
}

#endif // !CHANGEABLESOLIDOBJECT
