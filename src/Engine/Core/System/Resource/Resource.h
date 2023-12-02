#ifndef RESOURCE_H
#define RESOURCE_H

#include "Engine/EngineDef.h"

namespace Engine {
	class Resource {
	public:
		template<class TResourceClass>
		static TResourceClass Load(const String& filename);
	};
}

#endif // !RESOURCE_H
