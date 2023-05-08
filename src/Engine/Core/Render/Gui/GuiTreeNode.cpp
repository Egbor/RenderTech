#include "Engine/Core/Render/Gui/GuiTreeNode.h"

#include "imgui/imgui.h"

namespace Engine {
	GuiTreeNode::GuiTreeNode(const String& tag, const String& label, void* id) 
		: GuiLayout(tag), m_label(label), m_id(id), m_isSelected(false) {

	}

	void GuiTreeNode::SetActive(bool value) {
		m_isSelected = value;
	}

	bool GuiTreeNode::GetActive() const {
		return m_isSelected;
	}

	void* GuiTreeNode::GetId() const {
		return m_id;
	}

	void GuiTreeNode::Render(void* layoutData) {
		ImGuiTreeNodeFlags flags = (m_isSelected) ? ImGuiTreeNodeFlags_Selected : 0;
		if (ImGui::TreeNodeEx(m_id, flags | ImGuiTreeNodeFlags_DefaultOpen| ImGuiTreeNodeFlags_OpenOnArrow, m_label.c_str())) {
			if (ImGui::IsItemClicked()) {
				m_eventOnClick(this);
			}

			RenderChildLayouts(layoutData);
			ImGui::TreePop();
		}
	}

	GuiTree::GuiTree(const String& tag, const String& label, void* id) 
		: GuiTreeNode(tag, label, id), m_selected(nullptr), m_eventOnSelected() {
		AddOnClickEvent(Delegate<GuiTree, GuiLayout*>::Allocate(this, &GuiTree::OnNodeClicked));
	}

	bool GuiTree::AddTreeNode(GuiTree* node) {
		if (AddChildLayout(node)) {
			node->AddOnClickEvent(Delegate<GuiTree, GuiLayout*>::Allocate(this, &GuiTree::OnNodeClicked));
			node->AddOnSelectedEvent(Delegate<GuiTree, GuiLayout*, GuiTree*>::Allocate(this, &GuiTree::OnNodeSelected));
			return true;
		}
		return false;
	}

	bool GuiTree::RemoveTreeNode(const String& tag, GuiTree** outNode) {
		GuiTree* layout = nullptr;
		if (RemoveChildLayout(tag, reinterpret_cast<GuiLayout**>(&layout))) {
			layout->RemoveOnClickEvent(Delegate<GuiTree, GuiLayout*>::Allocate(this, &GuiTree::OnNodeClicked));
			layout->RemoveOnSelectedEvent(Delegate<GuiTree, GuiLayout*, GuiTree*>::Allocate(this, &GuiTree::OnNodeSelected));

			if (outNode != nullptr) {
				*outNode = layout;
			}

			return true;
		}
		return false;
	}

	void GuiTree::AddOnSelectedEvent(EventBase<GuiLayout*, GuiTree*>& callback) {
		m_eventOnSelected += callback;
	}

	void GuiTree::RemoveOnSelectedEvent(EventBase<GuiLayout*, GuiTree*>& callback) {
		m_eventOnSelected -= callback;
	}

	void GuiTree::OnNodeClicked(GuiLayout* owner) {
		GuiTree* node = dynamic_cast<GuiTree*>(owner);
		
		if (m_selected != nullptr) {
			m_selected->SetActive(false);
		}
		m_selected = node;
		m_selected->SetActive(true);

		OnNodeSelected(this, m_selected);
	}

	void GuiTree::OnNodeSelected(GuiLayout* owner, GuiTree* node) {
		m_eventOnSelected(owner, node);
	}
}