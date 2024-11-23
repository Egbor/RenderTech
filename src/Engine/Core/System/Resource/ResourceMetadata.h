#ifndef RESOURCE_METADATA_H
#define RESOURCE_METADATA_H

#include "Engine/Object/Object.h"

namespace Engine {
	class IResourceMetadata {
	public:
		virtual ~IResourceMetadata() = default;
		virtual IResourceMetadata* _SetName(const String& name) = 0;
		virtual Object* Build() = 0;
	};
}

#endif // !RESOURCE_METADATA_H