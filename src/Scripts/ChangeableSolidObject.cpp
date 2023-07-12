#include "Scripts/ChangeableSolidObject.h"

namespace Engine {
	GENERATE_INSTANTIATION(ChangeableSolidObject)

	ChangeableSolidObject::ChangeableSolidObject(const ObjectArgument& arguments) 
		: Actor(arguments), m_meshComponent(nullptr) {

	}
}