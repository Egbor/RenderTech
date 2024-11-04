#ifndef RESOURCE_HIERARCHY_H
#define RESOURCE_HIERARCHY_H

#include "Engine/Object/Object.h"

namespace Engine {
	struct ResourceItem {
		String name;
		Object* resource;
	};

	class ResourceNode {
	public:
		ResourceNode(const String& name);
		~ResourceNode();

		Object* Get(const String& path);
		void Set(const String& path, Object* obj);

		const String& Name() const;
		const String& Filename(const String& path) const;

	private:
		String m_name;

		Array<ResourceItem> m_relatedResources;
		Array<ResourceNode*> m_relatedNodes;
	};
}

#endif // !RESOURCE_HIERARCHY_H
