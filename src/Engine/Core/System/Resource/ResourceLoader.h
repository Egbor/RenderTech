#ifndef RESOURCE_LOADER_H
#define RESOURCE_LOADER_H

#include "Engine/EngineDef.h"
#include "Engine/Core/System/Resource/ResourceHierarchy.h"

namespace Engine {
	//template<class TObjectClass>
	//TObjectClass* LoadResourceFromFile(const String& filename);

	//template<class TObjectClass>
	//void SaveResourceInFile(const String& filename, TObjectClass* resource);

	class ResourceLoader {
	public:
		template<class TObjectClass>
		static void Load(ResourceNode* root, const String& path, TObjectClass* outResource);

		template<class TObjectClass>
		static void Save(ResourceNode* root, const String& path, TObjectClass* inResource);
	};
}

#endif // !RESOURCE_LOADER_H
