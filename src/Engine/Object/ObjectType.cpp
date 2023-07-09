#include "Engine/Object/ObjectType.h"

namespace Engine {
	ObjectField::ObjectField(ObjectType* owner, ObjectType* type, const String& name)
		: m_name(name), m_owner(owner), m_type(type) {
		owner->IncludeFieldInList(this);
	}

	ObjectType* ObjectField::GetOwner() const {
		return m_owner;
	}

	ObjectType* ObjectField::GetType() const {
		return m_type;
	}

	String ObjectField::GetName() const {
		return m_name;
	}

	ObjectType::ObjectType(const String& name) 
		: m_name(name), m_fields(), m_methods() {
		InitializeType(m_name, this);
	}

	void ObjectType::IncludeFieldInList(ObjectField* field) {
		m_fields.push_back(field);
	}

	ObjectField* ObjectType::GetField(const String& name) const {
		for (auto it = m_fields.begin(); it != m_fields.end(); it++) {
			if ((*it)->GetName() == name) {
				return (*it);
			}
		}
		return nullptr;
	}

	ObjectMethod* ObjectType::GetMethod(const String& name) const {
		return nullptr;
	}

	bool ObjectType::IsClass() const {
		return false;
	}

	bool ObjectType::IsEqual(const ObjectType& type) const {
		return type.m_name == m_name;
	}

	String ObjectType::GetName() const {
		return m_name;
	}

	Map<String, ObjectType*>& ObjectType::GetTypeTable() {
		static Map<String, ObjectType*> table;
		return table;
	}

	Map<String, void*>& ObjectType::GetPropertyTable(BaseObject* owner) {
		return owner->m_properties;
	}

	void ObjectType::InitializeType(const String& type, ObjectType* owner) {
		Map<String, ObjectType*>& types = GetTypeTable();
		types[type] = owner;
	}
}