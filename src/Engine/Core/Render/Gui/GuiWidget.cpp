#include "Engine/Core/Render/Gui/GuiWidget.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

namespace Engine {
	GuiWidget::GuiWidget(const String& tag, const String& name) 
		: GuiLayout(tag), m_name(name), m_open(true), m_width(800), m_height(600) {
		Show();
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

	void GuiWidget::AddOnCloseEvent(EventBase<GuiLayout*>& callback) {
		m_eventOnClose += callback;
	}

	void GuiWidget::AddOnShowEvent(EventBase<GuiLayout*>& callback) {
		m_eventOnShow += callback;
	}

	void GuiWidget::AddOnResizeEvent(EventBase<GuiLayout*, Int32, Int32>& callback) {
		m_eventOnResize += callback;
	}

	void GuiWidget::RemoveOnCloseEvent(EventBase<GuiLayout*>& callback) {
		m_eventOnClose -= callback;
	}

	void GuiWidget::RemoveOnShowEvent(EventBase<GuiLayout*>& callback) {
		m_eventOnShow -= callback;
	}

	void GuiWidget::RemoveResizeEvent(EventBase<GuiLayout*, Int32, Int32>& callback) {
		m_eventOnResize -= callback;
	}

	void GuiWidget::Show() {
		m_open = true;
		m_eventOnShow(this);
	}

	void GuiWidget::Render(void* layoutData) {
		if (m_open) {
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			if (ImGui::Begin(m_name.c_str(), &m_open)) {
				if (ImGuiDockNode* dockNode = ImGui::DockBuilderGetNode(ImGui::GetWindowDockID())) {
					dockNode->LocalFlags &= ~ImGuiDockNodeFlags_CentralNode;
				}

				HandleOnResizeEvent();
				HandleOnClickEvent();
				HandleOnCloseEvent();

				RenderChildLayouts(layoutData);

			}
			ImGui::End();
			ImGui::PopStyleVar();
		}
	}

	void GuiWidget::HandleOnClickEvent() {
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
			m_eventOnClick(this);
		}
	}

	void GuiWidget::HandleOnResizeEvent() {
		ImVec2 curWidgetSize = ImGui::GetContentRegionAvail();
		if (curWidgetSize.x != m_width || curWidgetSize.y != m_height) {
			m_width = curWidgetSize.x;
			m_height = curWidgetSize.y;
			m_eventOnResize(this, m_width, m_height);
		}
	}

	void GuiWidget::HandleOnCloseEvent() {
		if (!m_open) {
			m_eventOnClose(this);
		}
	}
}