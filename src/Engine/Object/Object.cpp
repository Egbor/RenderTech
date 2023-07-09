#include "Engine/Object/Object.h"
#include "Engine/Core/System/Exception/EngineException.h"

namespace Engine {
    //GENERATE_RTTI_DEFINITIONS(Object)
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

    void Object::Serialize(ISerializer* serializer) {

    }

    void Object::Deserialize(ISerializer* serializer) {
        if (!Is(serializer->GetObjectName())) {
            throw EngineException("[Object] Object::Deserialize() failed.");
        }
        serializer->GetString(argTagName, m_name);
    }
}
