#ifndef RESOURCE_LOADER_H
#define RESOURCE_LOADER_H

#include "Engine/EngineDef.h"

namespace Engine {
	class ResourceLoader {
	public:
		static Object* Load(const String& path);
		static void Save(const String& path, Object* object);
	};
}

#endif // !RESOURCE_LOADER_H
