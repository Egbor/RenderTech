#include "Engine/Object/Object.h"
#include "Engine/Core/System/Exception/EngineException.h"

namespace Engine {
    GENERATE_INSTANTIATION(Object)

    Object::Object(const ObjectArgument& argument) 
        : m_name("None") {
        argument.Pull(argTagName, m_name);
    }

    void Object::SetName(const String& name) {
        m_name = name;
    }

    const String& Object::GetName() const {
        return m_name;
    }
}
