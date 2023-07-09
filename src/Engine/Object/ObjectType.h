#ifndef OBJECTTYPE
#define OBJECTTYPE

#include "Engine/Object/BaseObject.h"
#include "Engine/Core/System/Exception/EngineException.h"

#include "Engine/Math/Vector.h"
#include "Engine/Math/Rotator.h"

namespace Engine { 
    class ObjectType {
    private:
        friend class ObjectField;
        friend class ObjectMethod;

    public:
        ObjectType(const String& name);
        virtual ~ObjectType() = default;

        ObjectField* GetField(const String& name) const;
        ObjectMethod* GetMethod(const String& name) const;

        String GetName() const;

        virtual bool IsClass() const;
        virtual bool IsEqual(const ObjectType& type) const;

        virtual void* CreateInstance() = 0;

    protected:
        void InitializeType(const String& type, ObjectType* owner);
        void IncludeFieldInList(ObjectField* field);

    private:
        static Map<String, ObjectType*>& GetTypeTable();
        static Map<String, void*>& GetPropertyTable(BaseObject* owner);

        const String m_name;

        Array<ObjectField*> m_fields;
        Array<ObjectMethod*> m_methods;

    public:
        template<typename TType>
        static TType* CreateDefaultObjectByName(const String& name) {
            try {
                const Map<String, ObjectType*>& table = GetTypeTable();
                return reinterpret_cast<TType*>(table.at(name)->CreateInstance());
            } catch (std::out_of_range) {
                throw EngineException("[ObjectType] CreateDefaultObjectByName() failed.");
            }
        }
    };

    template<typename TType>
    class ObjectValueType;

    template<typename TType>
    class ObjectClassType;

#define VALUETYPE(Type)                                             \
    template<>                                                      \
    class ObjectValueType<Type> : public ObjectType {               \
    private:                                                        \
        ObjectValueType() : ObjectType(#Type) {}                    \
        virtual ~ObjectValueType() = default;                       \
                                                                    \
    public:                                                         \
        bool IsClass() const override {                             \
            return false;                                           \
        }                                                           \
                                                                    \
        static Type* CreateDefaultObject() {                        \
            return new Type();                                      \
        }                                                           \
                                                                    \
        static ObjectType* GetInstance() {                          \
            static ObjectType* type = new ObjectValueType<Type>();  \
            return type;                                            \
        }                                                           \
                                                                    \
    private:                                                        \
        void* CreateInstance() override {                           \
            return CreateDefaultObject();                           \
        }                                                           \
    };

#define CLASSTYPE(Type)                                             \
    class Type;                                                     \
                                                                    \
    template<>                                                      \
    class ObjectClassType<Type> : public ObjectType {               \
    private:                                                        \
        ObjectClassType() : ObjectType(#Type) {}                    \
        virtual ~ObjectClassType() = default;                       \
                                                                    \
    public:                                                         \
                                                                    \
        bool IsClass() const override {                             \
            return true;                                            \
        }                                                           \
                                                                    \
        static Type* CreateDefaultObject(const ObjectArgument&);    \
        static Type* CreateDefaultObject();                         \
        static ObjectType* GetInstance();                           \
                                                                    \
    private:                                                        \
        void* CreateInstance() override {                           \
            return CreateDefaultObject();                           \
        }                                                           \
                                                                    \
    };

#define GENERATE_CLASSTYPE_DEFINITIONS(Type)                                                \
        Type* ObjectClassType<Type>::CreateDefaultObject(const ObjectArgument& arguments) { \
            return new Type(arguments);                                                     \
        }                                                                                   \
                                                                                            \
        Type* ObjectClassType<Type>::CreateDefaultObject() {                                \
            return CreateDefaultObject(ObjectArgument::Dummy());                            \
        }                                                                                   \
                                                                                            \
        ObjectType* ObjectClassType<Type>::GetInstance() {                                  \
            static ObjectType* type = new ObjectClassType<Type>();                          \
            return type;                                                                    \
        }

#define GENERATE_INSTANTIATION(Type)     \
    GENERATE_RTTI_DEFINITIONS(Type)      \
    GENERATE_CLASSTYPE_DEFINITIONS(Type)

    VALUETYPE(Int8);
    VALUETYPE(Int16);
    VALUETYPE(Int32);
    VALUETYPE(Int64);
    VALUETYPE(UInt8);
    VALUETYPE(UInt16);
    VALUETYPE(UInt32);
    VALUETYPE(UInt64);
    VALUETYPE(Float);
    VALUETYPE(Double);
    VALUETYPE(String);

    VALUETYPE(Vector3);
    VALUETYPE(Vector4);
    VALUETYPE(Rotator);

    class ObjectField {
    private:
        ObjectType* m_owner;
        ObjectType* m_type;
        String m_name;

    public:
        ObjectField(ObjectType* owner, ObjectType* type, const String& name);
        virtual ~ObjectField() = default;

        ObjectType* GetOwner() const;
        ObjectType* GetType() const;
        String GetName() const;

        template<class TType>
        void Set(BaseObject* owner, TType value) {
            TType* field = reinterpret_cast<TType*>(ObjectType::GetPropertyTable(owner)[m_name]);
            *field = value;
        }

        template<class TType>
        TType Get(BaseObject* owner) {
            TType* field = reinterpret_cast<TType*>(ObjectType::GetPropertyTable(owner)[m_name]);
            return (*field);
        }
    };

    class ObjectMethod {
    public:
        ObjectMethod() = default;
        virtual ~ObjectMethod() = default;
    };
}

#endif // !OBJECTTYPE
