#include "Engine/Core/Render/Gui/GuiTreeNode.h"

#include "imgui/imgui.h"

namespace Engine {
	GuiTreeNode::GuiTreeNode(const String& tag, const String& label, const void* id) 
		: GuiLayout(tag), m_label(label), m_id(id) {

	}

	void GuiTreeNode::Render(void* layoutData) {
		if (ImGui::TreeNodeEx(m_id, ImGuiTreeNodeFlags_OpenOnArrow, m_label.c_str())) {
			RenderChildLayouts(layoutData);
			ImGui::TreePop();
		}
	}
}