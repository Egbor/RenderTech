#include "Engine/Core/Render/Gui/GuiMenu.h"

#include "imgui/imgui.h"

namespace Engine {
	GuiMenu::GuiMenu(const String& tag, const String& label) 
		: GuiLayout(tag), m_label(label), m_menuItems() {

	}

	GuiMenu::~GuiMenu() {
		while (m_menuItems.begin() != m_menuItems.end()) {
			RemoveMenuItem(m_menuItems.begin()->first);
		}
	}

	GuiMenuItem* GuiMenu::AddMenuItem(const String& tag, const String& label) {
		MapIterator<String, GuiMenuItem*> it = m_menuItems.find(tag);
		if (it == m_menuItems.end()) {
			m_menuItems[tag] = new GuiMenuItem(tag, label);
			AddChildLayout(m_menuItems[tag]);
		}
		return m_menuItems[tag];
	}

	void GuiMenu::RemoveMenuItem(const String& tag) {
		MapIterator<String, GuiMenuItem*> it = m_menuItems.find(tag);
		if (it != m_menuItems.end()) {
			RemoveChildLayout(tag);
			DELETE_OBJECT(m_menuItems[tag]);
			m_menuItems.erase(tag);
		}
	}

	void GuiMenu::Render(void* layoutData) {
		if (ImGui::BeginMenu(m_label.c_str())) {
			RenderChildLayouts(layoutData);
			ImGui::EndMenu();
		}
	}

	String GuiMenu::GetLabel() const {
		return m_label;
	}
}