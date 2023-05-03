#include "Engine/Core/Render/Gui/GuiWidget.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

namespace Engine {
	GuiWidget::GuiWidget(const String& tag, const String& name) 
		: GuiLayout(tag), m_name(name), m_width(800), m_height(600) {

	}

	String GuiWidget::GetName() const {
		return m_name;
	}

	Int32 GuiWidget::GetWidth() const {
		return m_width;
	}

	Int32 GuiWidget::GetHeight() const {
		return m_height;
	}

	void GuiWidget::AddOnResizeEvent(EventBase<GuiLayout*, Int32, Int32>& callback) {
		m_eventOnResize += callback;
	}

	void GuiWidget::RemoveResizeEvent(EventBase<GuiLayout*, Int32, Int32>& callback) {
		m_eventOnResize -= callback;
	}

	void GuiWidget::Render() {	
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin(m_name.c_str());
		if (ImGuiDockNode* dockNode = ImGui::DockBuilderGetNode(ImGui::GetWindowDockID())) {
			dockNode->LocalFlags &= ~ImGuiDockNodeFlags_CentralNode;
		}

		ImVec2 curWidgetSize = ImGui::GetContentRegionAvail();
		if (curWidgetSize.x != m_width || curWidgetSize.y != m_height) {
			m_width = curWidgetSize.x;
			m_height = curWidgetSize.y;
			m_eventOnResize(this, m_width, m_height);
		}

		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
			m_eventOnClick(this);
		}

		RenderChildLayouts();

		ImGui::End();
		ImGui::PopStyleVar();
	}
}