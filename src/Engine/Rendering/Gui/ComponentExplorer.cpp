#include "Engine/Rendering/Gui/ComponentExplorer.h"

#include <sstream>

namespace Engine {
	GuiTree* CreateComponentTree(SceneComponent* component) {		
		const List<SceneComponent*>& children = component->GetChildComponents();

		std::stringstream ss;
		ss << (UInt64)component;

		GuiTree* gui = new GuiTree(ss.str(), component->GetName(), component);
		for (auto it = children.begin(); it != children.end(); it++) {
			gui->AddTreeNode(CreateComponentTree(*it));
		}
		return gui;
	}

	ComponentExplorer::ComponentExplorer(const String& name, const String& tag) 
		: GuiWidget(tag, name), m_root(new GuiTree("Components", "Components", nullptr)), m_rootTag() {
		AddChildLayout(m_root);
	}

	ComponentExplorer::~ComponentExplorer() {
		DELETE_LAYOUT(m_root);
	}

	void ComponentExplorer::SetComponentsToExplorer(Entity* entity) {
		GuiTree* root = nullptr;
		if (m_root->RemoveTreeNode(m_rootTag, reinterpret_cast<GuiTree**>(&root))) {
			DELETE_LAYOUT(root);
		}
		root = CreateComponentTree(entity->GetRootComponent());
		m_rootTag = root->GetTag();
		m_root->AddTreeNode(root);
	}

	void ComponentExplorer::AddOnComponentSelectedEvent(EventBase<GuiLayout*, GuiTree*>& callback) {
		m_root->AddOnSelectedEvent(callback);
	}

	void ComponentExplorer::RemoveOnComponentSelectedEvent(EventBase<GuiLayout*, GuiTree*>& callback) {
		m_root->RemoveOnSelectedEvent(callback);
	}
}