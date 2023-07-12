#include "Engine/Object/ObjectType.h"

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