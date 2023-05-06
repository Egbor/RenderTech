#include "Engine/Core/Render/Gui/GuiDragInput.h"

#include "imgui/imgui.h"

namespace Engine {
	GuiDragInput::GuiDragInput(const String& tag, const String& label) 
		: GuiLayout(tag), m_label(label), m_data(nullptr) {

	}

	void GuiDragInput::SetData(Float* data) {
		m_data = data;
	}

	void GuiDragInput::Render(void* layoutData) {
		static float plug[] = { 0.0f, 0.0f, 0.0f };
		ImGui::DragFloat3(m_label.c_str(), (m_data == nullptr) ? plug : m_data, 0.001f);
	}
}