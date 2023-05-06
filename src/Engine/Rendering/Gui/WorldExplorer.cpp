#include "Engine/Rendering/Gui/WorldExplorer.h"

#include <sstream>

namespace Engine {
	WorldExplorer::WorldExplorer(const String& name, const String& tag) 
		: GuiWidget(tag, name) {
		m_root = new GuiTreeNode("Scene", "Scene", nullptr);
		AddChildLayout(m_root);
	}

	WorldExplorer::~WorldExplorer() {
		DELETE_LAYOUT(m_root);
	}

	void WorldExplorer::AddEntityToExplorer(Entity* entity) {
		std::stringstream ss;
		ss << (UInt64)entity;

		m_root->AddChildLayout(new GuiTreeNode(ss.str(), entity->GetName(), entity));
	}

	void WorldExplorer::RemoveEntityFromExplorer(Entity* entity) {
		std::stringstream ss;
		ss << (UInt64)entity;

		GuiLayout* layout = nullptr;
		m_root->RemoveChildLayout(ss.str(), &layout);
		DELETE_LAYOUT(layout);
	}
}