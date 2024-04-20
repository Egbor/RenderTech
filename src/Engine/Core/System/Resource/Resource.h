#ifndef RESOURCE_H
#define RESOURCE_H

#include "Engine/EngineDef.h"

namespace Engine {
	enum class ResourceStatus {
		RS_OK,
		RS_FAILED
	};

	class Resource {
	public:
		template<class TResourceClass>
		static TResourceClass Load(const String& filename);
		
		template<class TResourceClass>
		static ResourceStatus Save(TResourceClass resource, const String& filename);
	};
}

#endif // !RESOURCE_H
