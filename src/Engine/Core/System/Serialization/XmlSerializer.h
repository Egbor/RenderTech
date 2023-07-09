#ifndef XMLSERIALIZER
#define XMLSERIALIZER

#include "Engine/Core/System/Serialization/ISerializer.h"

#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_utils.hpp"

namespace Engine {
	class XmlSerializer : public ISerializer {
	private:
		rapidxml::file<>* m_file;
		rapidxml::xml_node<>* m_node;

		XmlSerializer(rapidxml::xml_node<>* node);

	public:
		XmlSerializer(const String& filename, SerializationMode mode);
		virtual ~XmlSerializer();

		virtual String GetObjectName() const override;

		bool GetInt8(const String& name, Int8& outValue) const override;
		bool GetInt16(const String& name, Int16& outValue) const override;
		bool GetInt32(const String& name, Int32& outValue) const override;
		bool GetInt64(const String& name, Int64& outValue) const override;
		bool GetFloat(const String& name, Float& outValue) const override;
		bool GetDouble(const String& name, Double& outValue) const override;
		bool GetString(const String& name, String& outValue) const override;

		bool CreateSubobjectSerializer(ISerializer** serializer) const override;
		bool CreateSubobjectSerializer(const String& filename, ISerializer** serializer) const override;
		void DispatchSubobjectSerializer(ISerializer** serializer) const override;

		Object* Read() override;
		void Write() override;
	};
}

#endif // !XMLSERIALIZER
