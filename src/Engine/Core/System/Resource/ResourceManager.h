#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "Engine/Core/System/Resource/ResourceLoader.h"
#include "Engine/Core/System/Resource/ResourceHierarchy.h"

namespace Engine {
	class ResourceManager {
	public:
		ResourceManager() noexcept : m_root(new ResourceNode("assets")) {}
		~ResourceManager() noexcept { DELETE_OBJECT(m_root); }

		template<class TObjectClass>
		TObjectClass* Load(const String& path) const {
			Object* resource = m_root->Get(path);
			if (!resource) {
				resource = ResourceLoader::Load(m_root->Name() + "\\" + path);
				m_root->Set(path, resource);
			}
			return resource->As<TObjectClass>();
		}

	private:
		ResourceNode* m_root;
	};
}

#endif // !RESOURCE_MANAGER_H
