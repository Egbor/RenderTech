#include "Engine/Core/System/Resource/ResourceHierarchy.h"
#include "Engine/Core/System/Exception/EngineException.h"

namespace Engine {
	ResourceNode::ResourceNode(const String& name) 
		: m_name(name), m_relatedResources(), m_relatedNodes() {
	}

	ResourceNode::~ResourceNode() {
		DELETE_ARRAY_OF_OBJECTS(m_relatedNodes);
		for (Int32 i = 0; i < static_cast<Int32>(m_relatedResources.size()); i++) {
			DELETE_OBJECT(m_relatedResources[i].resource);
		}
	}

	Object* ResourceNode::Get(const String& path) {
		auto nameIterator = std::find_if(path.begin(), path.end(), [](const char& item) { item == '/' || item == '\\'; });

		if (nameIterator == path.end()) {
			for (Int32 i = 0; i < static_cast<Int32>(m_relatedResources.size()); i++) {
				if (strncmp(m_relatedResources[i].name.c_str(), path.c_str(), path.size()) == 0) {
					return m_relatedResources[i].resource;
				}
			}
		} else {
			for (Int32 i = 0; i < static_cast<Int32>(m_relatedNodes.size()); i++) {
				if (strncmp(m_relatedNodes[i]->m_name.c_str(), path.c_str(), nameIterator - path.begin()) == 0) {
					return m_relatedNodes[i]->Get(String(nameIterator + 1, path.end()));
				}
			}
		}

		return nullptr;
	}

	void ResourceNode::Set(const String& path, Object* obj) {
		auto nameIterator = std::find_if(path.begin(), path.end(), [](const char& item) { item == '/' || item == '\\'; });

		if (nameIterator == path.end()) {
			m_relatedResources.push_back({ path, obj });
		} else {
			for (Int32 i = 0; i < static_cast<Int32>(m_relatedNodes.size()); i++) {
				if (strncmp(m_relatedNodes[i]->m_name.c_str(), path.c_str(), nameIterator - path.begin()) == 0) {
					m_relatedNodes[i]->Set(String(nameIterator + 1, path.end()), obj);
					return;
				}
			}

			m_relatedNodes.push_back(new ResourceNode(String(path.begin(), nameIterator)));
			m_relatedNodes[m_relatedNodes.size() - 1]->Set(String(nameIterator + 1, path.end()), obj);
		}
	}

	const String& ResourceNode::Name() const {
		return m_name;
	}

	const String& ResourceNode::Filename(const String& path) const {
		return m_name + "/" + path;
	}
}