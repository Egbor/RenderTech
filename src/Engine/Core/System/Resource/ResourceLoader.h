#ifndef RESOURCE_LOADER_H
#define RESOURCE_LOADER_H

#include "Engine/EngineDef.h"
#include "Engine/Core/System/Resource/ResourceHierarchy.h"

namespace Engine {
	class ResourceLoader {
	public:
		static Object* Load(const String& path);
	};
}

#endif // !RESOURCE_LOADER_H
