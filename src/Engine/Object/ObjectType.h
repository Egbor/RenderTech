#ifndef OBJECTTYPE
#define OBJECTTYPE

#include "Engine/EngineDef.h"

namespace Engine {
    enum class TypeEnum {
        TE_VALUE = 0,
        TE_CLASS = 1,
        TE_ARRAY = 2
    };

    class BaseObject;
    class ObjectArgument;

    class IType {
    public:
        virtual String IType_GetName() const = 0;
        virtual bool Is(TypeEnum type) const = 0;
    };

    class IClass {
        friend class FieldInfo;
        friend class MethodInfo;

    public:
        virtual BaseObject* CreateDefaultObject(const ObjectArgument& argument) = 0;

        virtual String IClass_GetName() const = 0;
        virtual FieldInfo* GetField(const String& name) const = 0;
        virtual MethodInfo* GetMethod(const String& name) const = 0;

    protected:
        virtual void RegisterField(FieldInfo* field) = 0;
    };

    class TypeMap {
    private:
        TypeMap() = default;
        virtual ~TypeMap() = default;

    public:
        void Set(const String& name, IType* type);
        IType* GetAsType(const String& name);
        IClass* GetAsClass(const String& name);

        static TypeMap* GetInstance();

    private:
        Map<String, IType*> m_types;
    };

    template<typename TType>
    class CommonType : public IType {
    private:
        const TypeEnum m_type;

    protected:
        CommonType(TypeEnum type) 
            : m_type(type) {
            TypeMap::GetInstance()->Set(TypeName<TType>::name, this);
        }

        virtual ~CommonType() = default;

    public:
        String IType_GetName() const override {
            return TypeName<TType>::name;
        }

        bool Is(TypeEnum type) const override {
            return m_type == type;
        }
    };

    template<typename TType>
    class ValueType : public CommonType<TType> {
    private:
        ValueType() 
            : CommonType<TType>(TypeEnum::TE_VALUE) {

        }

        virtual ~ValueType() = default;

    public:
        static ValueType<TType>* GetInstance() {
            static ValueType<TType>* type = new ValueType<TType>();
            return type;
        }
    };

    template<typename TType>
    class ClassType : public CommonType<TType>, public IClass {
    private:
        ClassType() 
            : CommonType<TType>(TypeEnum::TE_CLASS)
            , m_fields(), m_methods(), m_base(TType::Super::TypeClass()) {

        }

        virtual ~ClassType() = default;

    public:
        BaseObject* CreateDefaultObject(const ObjectArgument& arguments) {
            return new TType(arguments);
        }

        String IClass_GetName() const override {
            return IType_GetName();
        }

        FieldInfo* GetField(const String& name) const override {
            auto it = std::find_if(m_fields.begin(), m_fields.end(), [&](FieldInfo* field) { return field->GetName() == name; });
            if (it != m_fields.end()) {
                return (*it);
            }
            if (m_base != nullptr) {
                return m_base->GetField(name);
            }
            return nullptr;
        }

        MethodInfo* GetMethod(const String& name) const override {
            return nullptr;
        }

        static ClassType<TType>* GetInstance() {
            static ClassType<TType>* type = new ClassType<TType>();
            return type;
        }

        static TType* CreateObject(const ObjectArgument& arguments) {
            return GetInstance()->CreateDefaultObject(arguments)->As<TType>();
        }

    private:
        void RegisterField(FieldInfo* field) override {
            m_fields.push_back(field);
        }

    private:
        IClass* m_base;

        Array<FieldInfo*> m_fields;
        Array<MethodInfo*> m_methods;
    };

    template<typename TType>
    class ArrayType : public CommonType<Array<TType>> {
    private:
        ArrayType() 
            : CommonType<TType>(TypeEnum::TE_ARRAY) {

        }

        virtual ~ArrayType() = default;

    public:
        static ArrayType<TType>* GetInstance() {
            static ArrayType<TType>* type = new ArrayType<TType>();
            return type;
        }
    };

    class FieldInfo {
    private:
        IClass* m_owner;
        IType* m_type;
        const String m_name;

    public:
        FieldInfo(IClass* owner, IType* type, const String& name);
        virtual ~FieldInfo() = default;

        template<typename TType>
        void Set(BaseObject* obj, TType value) {
            if (!obj->Is(m_owner->IClass_GetName())) {
                return;
            }
            TType* val = reinterpret_cast<TType*>(obj->m_propertyMap[m_name]);
            (*val) = value;
        }

        template<typename TType>
        TType Get(BaseObject* obj) {
            if (!obj->Is(m_owner->IClass_GetName())) {
                return nullptr;
            }
            TType* val = reinterpret_cast<TType*>(obj->m_propertyMap[m_name]);
            return (*val);
        }

        IClass* GetOwner() const;
        IType* GetType() const;
        String GetName() const;
    };

    class MethodInfo {

    };
}

#endif // !OBJECTTYPE
