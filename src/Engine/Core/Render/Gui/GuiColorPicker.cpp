#include "Engine/Core/Render/Gui/GuiColorPicker.h"

#include "imgui/imgui.h"

namespace Engine {
	GuiColorPicker::GuiColorPicker(const String& tag, const String& label) 
		: GuiLayout(tag), m_label(label), m_data(nullptr) {

	}

	void GuiColorPicker::SetData(Float* data) {
		m_data = data;
	}

	void GuiColorPicker::Render(void* layoutData) {
		static Float plug[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		ImGui::ColorEdit4(m_label.c_str(), (m_data == nullptr) ? plug : m_data, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_Float);
	}
}