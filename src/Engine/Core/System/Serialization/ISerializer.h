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

		virtual Object* Read() = 0;
		virtual void Write() = 0;
	};
}

#endif // !ISERIALIZER
