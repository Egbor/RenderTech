#ifndef OBJECT_H
#define OBJECT_H

#include "Engine/Object/MetaObject.h"
#include "Engine/Object/BaseObject.h"
#include "Engine/Object/ObjectArgument.h"

namespace Engine {
    class Object : public BaseObject {
        GENERATE_BODY(Object, BaseObject)

    private:
        String m_name;

    public:
        Object() = default;
        Object(const ObjectArgument& argument);
        Object(const Object&) = default;
        virtual ~Object() = default;

        const String& GetName() const;

        template<class TReturnClass>
        TReturnClass* CreateDefaultSubobject(const ObjectArgument& argument) {
            return TReturnClass::TypeMetaClass()->template CreateDefaultObject<TReturnClass>(argument);
        }

        template<class TReturnClass>
        TReturnClass* CreateDefaultSubobject(const String& name = "None") {
            ObjectArgument argument;
            argument.Put(argTagName, name);
            return CreateDefaultSubobject<TReturnClass>(argument);
        }
    };
}

#endif // OBJECT_H
