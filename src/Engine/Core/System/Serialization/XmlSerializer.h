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

	public:
		XmlSerializer(const String& filename, SerializationMode mode);
		virtual ~XmlSerializer();

		Object* Read() override;
		void Write() override;
	};
}

#endif // !XMLSERIALIZER
