#include "Engine/Core/Render/Gui/GuiMenuItem.h"

#include "imgui/imgui.h"

#include <Windows.h>

namespace Engine {
	GuiMenuItem::GuiMenuItem(const String& tag, const String& label) 
		: GuiLayout(tag), m_label(label) {

	}

	void GuiMenuItem::Render() {
		if (ImGui::MenuItem(m_label.c_str(), "")) {
			InvokeOnClickEvent();
		}
	}

	String GuiMenuItem::GetLabel() const {
		return m_label;
	}
}