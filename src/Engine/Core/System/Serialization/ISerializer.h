#ifndef ISERIALIZER
#define ISERIALIZER

#include "Engine/EngineDef.h"

namespace Engine {
	class Object;

	enum class SerializationMode {
		SM_READ,
		SM_WRITE
	};

	class ISerializer {
	public:
		ISerializer() = default;
		virtual ~ISerializer() = default;

		virtual String GetObjectName() const = 0;

		virtual bool GetInt8(const String& name, Int8& outValue) const = 0;
		virtual bool GetInt16(const String& name, Int16& outValue) const = 0;
		virtual bool GetInt32(const String& name, Int32& outValue) const = 0;
		virtual bool GetInt64(const String& name, Int64& outValue) const = 0;
		virtual bool GetFloat(const String& name, Float& outValue) const = 0;
		virtual bool GetDouble(const String& name, Double& outValue) const = 0;
		virtual bool GetString(const String& name, String& outValue) const = 0;

		virtual bool CreateSubobjectSerializer(ISerializer** serializer) const = 0;
		virtual bool CreateSubobjectSerializer(const String& filename, ISerializer** serializer) const = 0;
		virtual void DispatchSubobjectSerializer(ISerializer** serializer) const = 0;

		virtual Object* Read() = 0;
		virtual void Write() = 0;
	};
}

#endif // !ISERIALIZER
