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
        virtual TypeEnum GetType() const = 0;
        virtual Size GetSize() const = 0;
        virtual bool Is(TypeEnum type) const = 0;
    };

    class IArray {
    public:
        virtual String IArray_GetName() const = 0;
        virtual void Resize(void* arr, Size size) = 0;
        virtual void SetValue(void* arr, Size id, void* value) = 0;
        virtual void* GetValue(void* arr, Size id) = 0;
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
        constexpr CommonType(TypeEnum type) noexcept
            : m_type(type) {
            TypeMap::GetInstance()->Set(TypeName<TType>::name, this);
        }

        virtual ~CommonType() = default;

    public:
        String IType_GetName() const override {
            return TypeName<TType>::name;
        }

        Size GetSize() const override {
            return _GetSize();
        }

        TypeEnum GetType() const override {
            return m_type;
        }

        bool Is(TypeEnum type) const override {
            return m_type == type;
        }

    private:
        constexpr Size _GetSize() const noexcept {
            return (m_type == TypeEnum::TE_CLASS) ? sizeof(TType*) : sizeof(TType);
        }
    };

    template<typename TType>
    class ValueType : public CommonType<TType> {
    private:
        constexpr ValueType() noexcept 
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
    struct ClassOf {
        static IClass* value;
    };

    template<typename TType>
    class ClassType : public CommonType<TType>, public IClass {
    private:
        constexpr ClassType() noexcept
            : CommonType<TType>(TypeEnum::TE_CLASS)
            , m_fields(), m_methods(), m_base(ClassOf<TType::Super>::value) {

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
        const IClass* m_base;

        Array<FieldInfo*> m_fields;
        Array<MethodInfo*> m_methods;
    };

    template<typename TType>
    IClass* ClassOf<TType>::value = ClassType<TType>::GetInstance();
    
    template<>
    IClass* ClassOf<BaseObject>::value = nullptr;

    template<typename TType>
    class ArrayType : public CommonType<Array<TType>>, public IArray {
    private:
        constexpr ArrayType() noexcept
            : CommonType<TType>(TypeEnum::TE_ARRAY) {

        }

        virtual ~ArrayType() = default;

    public:
        String IArray_GetName() const override {
            return IType_GetName();
        }

        static ArrayType<TType>* GetInstance() {
            static ArrayType<TType>* type = new ArrayType<TType>();
            return type;
        }

        void Resize(Array<TType>* arr, Size size) {
            if (arr->size() < size) {
                arr->resize(size);
            }
        }

        void SetValue(Array<TType>* arr, Size id, TType* value) {
            (*arr)[id] = *value;
        }

        TType* GetValue(Array<TType>* arr, Size id) {
            return (*arr)[id];
        }

    private:
        void Resize(void* arr, Size size) override {
            Resize(reinterpret_cast<Array<TType>*>(arr), size);
        }

        void SetValue(void* arr, Size id, void* value) override {
            SetValue(reinterpret_cast<Array<TType>*>(arr), id, reinterpret_cast<TType*>(value));
        }

        void* GetValue(void* arr, Size id) override {
            return GetValue(reinterpret_cast<Array<TType>*>(arr), id);
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

        void Set(BaseObject* obj, void* value);
        void* Get(BaseObject* obj);

        IClass* GetOwner() const;
        IType* GetType() const;
        String GetName() const;
    };

    class MethodInfo {

    };
}

#endif // !OBJECTTYPE
