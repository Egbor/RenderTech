#include "Engine/Core/Render/Gui/GuiButton.h"

#include "imgui/imgui.h"

namespace Engine {
	GuiButton::GuiButton(const String& tag, const String& label)
		: GuiLayout(tag), m_label(label) {

	}

	void GuiButton::Render(void* layoutData) {
		if (ImGui::Button(m_label.c_str())) {
			m_eventOnClick(this);
		}
	}
}