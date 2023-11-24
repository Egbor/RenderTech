#include "Engine/Core/System/Serialization/XmlSerializer.h"
#include "Engine/Object/Object.h"

#include "Engine/Core/System/Import/MeshImport.h"
#include "Engine/Core/System/Import/TextureImport.h"

#include <functional>

namespace Engine {
	XmlSerializer::XmlSerializer(const String& filename, SerializationMode mode) 
		: m_file(new rapidxml::file<>(filename.c_str())), m_node(new rapidxml::xml_document<>()) {
		if (mode == SerializationMode::SM_READ) {
			rapidxml::xml_document<>* document = reinterpret_cast<rapidxml::xml_document<>*>(m_node);
			document->parse<0>(m_file->data());
		}
	}

	XmlSerializer::~XmlSerializer() {
		DELETE_OBJECT(m_file);
	}

#define XML_CASTINGMAP_PAIR(Type, CastFunc)								\
	{ #Type, [](const String& value, void* out) {						\
		Type val = static_cast<Type>(CastFunc(value));					\
		Type* dst = reinterpret_cast<Type*>(out);						\
		*dst = val;														\
	} }

//#define XML_IMPORTING_PAIR(Type, Importer)								\
//	{ #Type, [](const String& filename, Object** out) {					\
//		Importer importer(filename);									\
//		(*out) = importer.LoadResource();								\
//	} }

	Map<String, std::function<void(const String&, void*)>> xmlCastingMap = {
		XML_CASTINGMAP_PAIR(Int8, std::stoi),
		XML_CASTINGMAP_PAIR(Int16, std::stoi),
		XML_CASTINGMAP_PAIR(Int32, std::stoi),
		XML_CASTINGMAP_PAIR(Int64, std::stoll),
		XML_CASTINGMAP_PAIR(Float, std::stof),
		XML_CASTINGMAP_PAIR(Double, std::stod)
//		XML_CASTINGMAP_PAIR(String)
	};

	struct XmlFieldLocalStorage {
		FieldInfo* field;
		Int32 offset;
	} xmlFieldLocalStorage;

	void RunRTOField(const String& value, Object* target, Object* root) {
		xmlFieldLocalStorage.field = root->GetType()->GetField(value);
		//FieldInfo* field = root->GetType()->GetField(value);
		//field->Set(root, target);
	}

	void RunRTOFieldId(const String& value, Object* target, Object* root) {
		xmlCastingMap[TypeName<Int32>::name](value, &xmlFieldLocalStorage.offset);
	}

	void RunRTOName(const String& value, Object* target, Object* root) {
		target->SetName(value);
	}

	void RunIMPMesh(const String& value, Object* target, Object* root) {
		MeshImporter::ImportTo(value, target);
	}

	void SetRTOAsValue(Object* target, Object* root) {

	}

	void SetRTOAsObject(Object* target, Object* root) {
		xmlFieldLocalStorage.field->Set(root, target);
	}

	void SetRTOAsElement(Object* target, Object* root) {
		IArray* type = dynamic_cast<IArray*>(xmlFieldLocalStorage.field->GetType());
		type->SetValue(xmlFieldLocalStorage.field->Get(root), xmlFieldLocalStorage.offset, target);
	}

	//Map<String, std::function<void(const String&, Object**)>> xmlImportingMap = {
	//	XML_IMPORTING_PAIR(Mesh, MeshImport),
	//	XML_IMPORTING_PAIR(Texture2D, TextureImport)
	//};

	Map<String, std::function<void(const String&, Object*, Object*)>> xmlAttributeMap = {
		{ "rto:field", RunRTOField },
		{ "rto:fieldId", RunRTOFieldId },
		{ "rto:name", RunRTOName },
		{ "imp:mesh", RunIMPMesh }
	};

	Array<std::function<void(Object*, Object*)>> xmlSetterArray = {
		SetRTOAsValue,
		SetRTOAsObject,
		SetRTOAsElement
	};

	Object* ConvertXmlNodeToObject(rapidxml::xml_node<>* node) {
		IClass* type = dynamic_cast<IClass*>(TypeMap::GetInstance()->GetAsType(node->name()));
		return dynamic_cast<Object*>(type->CreateDefaultObject(ObjectArgument::Dummy()));
	}

	void DeserializeXmlAttribues(rapidxml::xml_attribute<>* xmlAttr, Object* rtoRoot, Object* rtoTarget) {
		xmlFieldLocalStorage.field = nullptr;
		for (rapidxml::xml_attribute<>* attr = xmlAttr; attr != nullptr; attr = attr->next_attribute()) {
			xmlAttributeMap[attr->name()](attr->value(), rtoTarget, rtoRoot);
		}
		if (xmlFieldLocalStorage.field != nullptr) {
			xmlSetterArray[static_cast<Int32>(xmlFieldLocalStorage.field->GetType()->GetType())](rtoTarget, rtoRoot);
		}
	}

	void DeserializeXmlChildNodes(rapidxml::xml_node<>* xmlRoot, Object* rtoRoot) {
		for (rapidxml::xml_node<>* node = xmlRoot->first_node(); node != nullptr; node = node->next_sibling()) {
			Object* target = ConvertXmlNodeToObject(node);
			DeserializeXmlAttribues(node->first_attribute(), rtoRoot, target);
			DeserializeXmlChildNodes(node, target);
		}
	}

	Object* XmlSerializer::Read() {
		rapidxml::xml_node<>* root = m_node->first_node();

		Object* obj = ConvertXmlNodeToObject(root);
		DeserializeXmlAttribues(root->first_attribute(), nullptr, obj);
		DeserializeXmlChildNodes(root, obj);
		return obj;
	}

	void XmlSerializer::Write() {

	}
}