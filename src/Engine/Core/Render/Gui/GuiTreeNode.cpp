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
		: GuiTreeNode(tag, label, id), m_selected(nullptr), m_eventOnSleceted() {
		AddOnClickEvent(Delegate<GuiTree, GuiLayout*>::Allocate(this, &GuiTree::OnNodeClicked));
	}

	bool GuiTree::AddTreeNode(GuiTreeNode* node) {
		if (AddChildLayout(node)) {
			node->AddOnClickEvent(Delegate<GuiTree, GuiLayout*>::Allocate(this, &GuiTree::OnNodeClicked));
			return true;
		}
		return false;
	}

	bool GuiTree::RemoveTreeNode(const String& tag, GuiTreeNode** outNode) {
		GuiTreeNode* layout = nullptr;
		if (RemoveChildLayout(tag, reinterpret_cast<GuiLayout**>(&layout))) {
			layout->RemoveOnClickEvent(Delegate<GuiTree, GuiLayout*>::Allocate(this, &GuiTree::OnNodeClicked));

			if (outNode != nullptr) {
				*outNode = layout;
			}

			return true;
		}
		return false;
	}

	void GuiTree::AddOnSelectedEvent(EventBase<GuiLayout*, GuiTreeNode*>& callback) {
		m_eventOnSleceted += callback;
	}

	void GuiTree::RemoveOnSelectedEvent(EventBase<GuiLayout*, GuiTreeNode*>& callback) {
		m_eventOnSleceted -= callback;
	}

	void GuiTree::OnNodeClicked(GuiLayout* owner) {
		GuiTreeNode* node = dynamic_cast<GuiTreeNode*>(owner);
		
		if (m_selected != nullptr) {
			m_selected->SetActive(false);
		}
		m_selected = node;
		m_selected->SetActive(true);

		m_eventOnSleceted(this, m_selected);
	}
}