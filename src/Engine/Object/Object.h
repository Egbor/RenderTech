#ifndef OBJECT_H
#define OBJECT_H

#include "Engine/Object/ObjectType.h"
#include "Engine/Object/BaseObject.h"
#include "Engine/Object/ObjectArgument.h"

#include "Engine/Core/System/Serialization/ISerializer.h"

namespace Engine {
    CLASSTYPE(Object)
    class Object : public BaseObject {
        GENERATE_BODY(Object, BaseObject)

    private:
        PROPERTY(ObjectValueType<String>, name)
        String m_name;

    public:
        Object(const ObjectArgument& argument);
        Object(const Object&) = default;
        virtual ~Object() = default;

        void SetName(const String& name);
        const String& GetName() const;

        virtual void Serialize(ISerializer* serializer);
        virtual void Deserialize(ISerializer* serializer);
    };
}

#endif // OBJECT_H
