#ifndef BASEOBJECT_H
#define BASEOBJECT_H

#include "Engine/EngineDef.h"

namespace Engine {
    class BaseObject {
    private:
        friend class ObjectType;

        Map<String, void*> m_properties;

    public:
        virtual const unsigned long long& TypeIdInstance() = 0;

        virtual BaseObject* QueryInterface(const unsigned long long id) const {
            return nullptr;
        }

        virtual bool Is(const unsigned long long id) const {
            return false;
        }

        virtual bool Is(const String& name) const {
            return false;
        }

        template<typename T>
        T* As() {
            if (T::TypeIdClass()) {
                return dynamic_cast<T*>(this);
            }
            return nullptr;
        }

    protected:
        void IncludeObjectFieldInRTTI(const String& name, void* address) {
            m_properties[name] = address;
        }
    };
}

#define MAX_RTTI_OBJECT_NAME_LENGTH 64
#define GENERATE_RTTI_DECLARATIONS(Type, ParentType)										\
    public:                                                                                 \
        static ObjectType* TypeClass() { return &rtti_metaClass; }                       \
        static String TypeName() { return String(rtti_objectName); }                        \
        static const unsigned long long& TypeIdClass() { return rtti_objectId; }			\
        virtual const unsigned long long& TypeIdInstance() override { return Type::TypeIdClass(); }	\
                                                                                            \
        virtual BaseObject* QueryInterface(const unsigned long long id) const override {				\
            if (id == rtti_objectId) {                                                      \
                return (Type*)this;                                                         \
            } else {                                                                        \
                return ParentType::QueryInterface(id);                                      \
            }                                                                               \
        }                                                                                   \
                                                                                            \
        virtual bool Is(const unsigned long long id) const override {                                \
            if (id == rtti_objectId) {                                                      \
                return true;                                                                \
            } else {                                                                        \
                return ParentType::Is(id);                                                  \
            }                                                                               \
        }                                                                                   \
                                                                                            \
        virtual bool Is(const std::string& name) const override {                                    \
            if (name == TypeName()) {                                                       \
                return true;                                                                \
            } else {                                                                        \
                return ParentType::Is(name);                                                \
            }                                                                               \
        }                                                                                   \
                                                                                            \
    private:                                                                                \
        using Super = ParentType;                                                           \
        using This = Type;                                                                  \
                                                                                            \
        static unsigned long long rtti_objectId;                                            \
        static char rtti_objectName[MAX_RTTI_OBJECT_NAME_LENGTH];                           \
        static ObjectType& rtti_metaClass;

#define GENERATE_RTTI_OBJECT_ID(Type)												\
    unsigned long long Type::rtti_objectId = (unsigned long long)& Type::rtti_objectId;

#define GENERATE_RTTI_OBJECT_NAME(Type)												\
    char Type::rtti_objectName[MAX_RTTI_OBJECT_NAME_LENGTH] = #Type;

#define GENERATE_RTTI_META_CLASS(Type)												\
    ObjectType& Type::rtti_metaClass = *ObjectClassType<Type>::GetInstance();

#define GENERATE_RTTI_DEFINITIONS(Type)	GENERATE_RTTI_OBJECT_ID(Type) GENERATE_RTTI_OBJECT_NAME(Type) GENERATE_RTTI_META_CLASS(Type)
#define GENERATE_RTTI_DEFINITIONS_TEMPLATED(Type) template<class T> GENERATE_RTTI_OBJECT_ID(Type<T>) template<class T> GENERATE_RTTI_OBJECT_NAME(Type<T>) template<class T> GENERATE_RTTI_META_CLASS(Type<T>)

#define GENERATE_BODY(Type, ParentType) GENERATE_RTTI_DECLARATIONS(Type, ParentType)
#define GENERATE_REFLECTION_POINTS(Type) GENERATE_RTTI_DEFINITIONS(Type)

#define PROPERTY(ObjType, Name)                                                                                                         \
    inline static const ObjectField field_##Name = ObjectField(ObjectClassType<This>::GetInstance(), ObjType::GetInstance(), #Name);    \
    struct _Property_##Name {                                                                                                           \
        _Property_##Name(This* obj) {                                                                                                   \
            obj->IncludeObjectFieldInRTTI(#Name, &(obj->m_##Name));                                                                     \
        }                                                                                                                               \
    } _property_##Name{this};

#endif // BASEOBJECT_H
