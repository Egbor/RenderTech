#include "Engine/Object/ObjectType.h"

#include "Engine/Object/BaseObject.h"
#include "Engine/Core/System/Exception/EngineException.h"

namespace Engine {
	void TypeMap::Set(const String& name, IType* type) {
		m_types[name] = type;

	}

	IType* TypeMap::GetAsType(const String& name) {
		return m_types[name];
	}

	IClass* TypeMap::GetAsClass(const String& name) {
		IType* type = GetAsType(name);
		if (!type->Is(TypeEnum::TE_CLASS)) {
			return nullptr;
		}
		return dynamic_cast<IClass*>(type);
	}

	TypeMap* TypeMap::GetInstance() {
		static TypeMap* map = new TypeMap();
		return map;
	}

	FieldInfo::FieldInfo(IClass* owner, IType* type, const String& name) 
		: m_owner(owner), m_type(type), m_name(name) {
		owner->RegisterField(this);
	}

	void FieldInfo::Set(BaseObject* obj, void* value) {
		if (!obj->Is(m_owner->IClass_GetName())) {
			throw EngineException("[FieldInfo] BasedObject::Is() failed");
		}
		std::memcpy(obj->m_propertyMap[m_name], value, m_type->GetSize());
	}

	void* FieldInfo::Get(BaseObject* obj) {
		if (!obj->Is(m_owner->IClass_GetName())) {
			throw EngineException("[FieldInfo] BasedObject::Is() failed");
		}
		return obj->m_propertyMap[m_name];
	}

	IClass* FieldInfo::GetOwner() const {
		return m_owner;
	}

	IType* FieldInfo::GetType() const {
		return m_type;
	}

	String FieldInfo::GetName() const {
		return m_name;
	}
}