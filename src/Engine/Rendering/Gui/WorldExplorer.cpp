#include "Engine/Rendering/Gui/WorldExplorer.h"

#include <sstream>

namespace Engine {
	WorldExplorer::WorldExplorer(const String& name, const String& tag) 
		: GuiWidget(tag, name) {
		m_root = new GuiTree("Scene", "Scene", nullptr);
		AddChildLayout(m_root);
	}

	WorldExplorer::~WorldExplorer() {
		DELETE_LAYOUT(m_root);
	}

	void WorldExplorer::AddEntityToExplorer(Entity* entity) {
		std::stringstream ss;
		ss << (UInt64)entity;

		m_root->AddTreeNode(new GuiTreeNode(ss.str(), entity->GetName(), entity));
	}

	void WorldExplorer::RemoveEntityFromExplorer(Entity* entity) {
		std::stringstream ss;
		ss << (UInt64)entity;

		GuiTreeNode* layout = nullptr;
		m_root->RemoveTreeNode(ss.str(), &layout);
		DELETE_LAYOUT(layout);
	}

	void WorldExplorer::AddOnEntitySelectedEvent(EventBase<GuiLayout*, GuiTreeNode*>& callback) {
		m_root->AddOnSelectedEvent(callback);
	}

	void WorldExplorer::RemoveOnEntitySelectedEvent(EventBase<GuiLayout*, GuiTreeNode*>& callback) {
		m_root->RemoveOnSelectedEvent(callback);
	}
}