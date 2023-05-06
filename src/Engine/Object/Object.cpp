#include "Engine/Object/Object.h"

namespace Engine {
    GENERATE_RTTI_DEFINITIONS(Object)

    Object::Object(const ObjectArgument& argument) 
        : m_name("None") {
        argument.Pull(argTagName, m_name);
    }

    const String& Object::GetName() const {
        return m_name;
    }
}
