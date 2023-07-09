#include "Engine/Core/System/Serialization/XmlSerializer.h"
#include "Engine/Object/Object.h"

#include <functional>
#include <string.h>

namespace Engine {
	XmlSerializer::XmlSerializer(const String& filename, SerializationMode mode) 
		: m_file(new rapidxml::file<>(filename.c_str())), m_node(new rapidxml::xml_document<>()) {
		if (mode == SerializationMode::SM_READ) {
			rapidxml::xml_document<>* document = reinterpret_cast<rapidxml::xml_document<>*>(m_node);
			document->parse<0>(m_file->data());
		}
	}

	XmlSerializer::XmlSerializer(rapidxml::xml_node<>* node)
		: m_file(nullptr), m_node(node) {
	}

	XmlSerializer::~XmlSerializer() {
		DELETE_OBJECT(m_file);
	}

	String XmlSerializer::GetObjectName() const {
		return m_node->name();
	}

	template<class T, class Predicate>
	bool CastXmlAttributeValue(rapidxml::xml_attribute<>* attribute, Predicate cast, T& out) {
		if (attribute != nullptr) {
			out = static_cast<T>(cast(attribute->value()));
			return true;
		}
		return false;
	}

	bool XmlSerializer::GetInt8(const String& name, Int8& outValue) const {
		return CastXmlAttributeValue<Int8>(m_node->first_attribute(name.c_str()), [&](const String& str) { return std::stoi(str); }, outValue);
	}

	bool XmlSerializer::GetInt16(const String& name, Int16& outValue) const {
		return CastXmlAttributeValue<Int16>(m_node->first_attribute(name.c_str()), [&](const String& str) { return std::stoi(str); }, outValue);
	}

	bool XmlSerializer::GetInt32(const String& name, Int32& outValue) const {
		return CastXmlAttributeValue<Int32>(m_node->first_attribute(name.c_str()), [&](const String& str) { return std::stoi(str); }, outValue);
	}

	bool XmlSerializer::GetInt64(const String& name, Int64& outValue) const {
		return CastXmlAttributeValue<Int64>(m_node->first_attribute(name.c_str()), [&](const String& str) { return std::stoll(str); }, outValue);
	}

	bool XmlSerializer::GetFloat(const String& name, Float& outValue) const {
		return CastXmlAttributeValue<Float>(m_node->first_attribute(name.c_str()), [&](const String& str) { return std::stof(str); }, outValue);
	}

	bool XmlSerializer::GetDouble(const String& name, Double& outValue) const {
		return CastXmlAttributeValue<Double>(m_node->first_attribute(name.c_str()), [&](const String& str) { return std::stod(str); }, outValue);
	}

	bool XmlSerializer::GetString(const String& name, String& outValue) const {
		return CastXmlAttributeValue<String>(m_node->first_attribute(name.c_str()), [&](const String& str) { return str; }, outValue);
	}

	bool XmlSerializer::CreateSubobjectSerializer(ISerializer** serializer) const {
		rapidxml::xml_node<>* node = nullptr;
		
		if (*serializer == nullptr) {
			node = m_node->first_node();
		} else {
			XmlSerializer* xmlSerializer = dynamic_cast<XmlSerializer*>((*serializer));
			node = xmlSerializer->m_node->next_sibling();
		}

		DispatchSubobjectSerializer(serializer);

		if (node != nullptr) {
			*serializer = new XmlSerializer(node);
			return true;
		}
		return false;
	}

	bool XmlSerializer::CreateSubobjectSerializer(const String& filename, ISerializer** serializer) const {
		DispatchSubobjectSerializer(serializer);
		*serializer = new XmlSerializer(filename, SerializationMode::SM_READ);
		return true;
	}

	void XmlSerializer::DispatchSubobjectSerializer(ISerializer** serializer) const {
		DELETE_OBJECT(*serializer);
	}

#define XML_SETTER_PAIR(Type, CastFunc)								\
	{ #Type, [](Object* owner, ObjectField* field, const String& value) {	\
		Type val = static_cast<Type>(CastFunc(value));						\
		field->Set<Type>(owner, val);										\
	} }

	Map<String, std::function<void(Object*, ObjectField*, const String&)>> xmlSetter = {
		XML_SETTER_PAIR(Int8, std::stoi),
		XML_SETTER_PAIR(Int16, std::stoi),
		XML_SETTER_PAIR(Int32, std::stoi),
		XML_SETTER_PAIR(Int64, std::stoll),
		XML_SETTER_PAIR(Float, std::stof),
		XML_SETTER_PAIR(Double, std::stod),
		XML_SETTER_PAIR(String)
	};

	Object* XmlSerializer::Read() {
		rapidxml::xml_node<>* node = m_node->first_node();
		rapidxml::xml_attribute<>* attr = node->first_attribute();

		Object* obj = ObjectType::CreateDefaultObjectByName<Object>(node->name());
		ObjectType* objType = obj->TypeClass();

		while (attr != nullptr) {
			ObjectField* field = objType->GetField(attr->name());
			ObjectType* fieldType = field->GetType();
			
			xmlSetter[fieldType->GetName()](obj, field, attr->value());
			attr = attr->next_attribute();
		}

		return obj;
	}

	void XmlSerializer::Write() {

	}
}