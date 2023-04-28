#ifndef METAOBJECT_H
#define METAOBJECT_H

#include "Engine/Object/BaseObject.h"
#include "Engine/Object/ObjectArgument.h"

namespace Engine {
    class MetaClass {
    public:
        MetaClass() = default;
        MetaClass(const MetaClass&) = default;
        virtual ~MetaClass() = default;

        virtual BaseObject* CreateDefaultObject(const ObjectArgument& argument) = 0;

        template<class TReturnClass>
        TReturnClass* CreateDefaultObject(const ObjectArgument& argument) {
            return dynamic_cast<TReturnClass*>(CreateDefaultObject(argument));
        }
    };

    template<class TObjectClass>
    class MetaObject : public MetaClass {
    public:
        MetaObject() = default;
        MetaObject(const MetaObject&) = default;
        virtual ~MetaObject() = default;

        BaseObject* CreateDefaultObject(const ObjectArgument& argument) override {
            return new TObjectClass(argument);
        }
    };
}

#endif // METAOBJECT_H
