#include "Engine/Core/Render/Gui/GuiMenuBar.h"

#include "imgui/imgui.h"

namespace Engine {
	GuiMenuBar::GuiMenuBar(const String& tag) 
		: GuiLayout(tag), m_menus() {

	}

	GuiMenuBar::~GuiMenuBar() {
		while (m_menus.begin() != m_menus.end()) {
			RemoveMenu(m_menus.begin()->first);
		}
	}

	GuiMenu* GuiMenuBar::AddMenu(const String& tag, const String& label) {
		MapIterator<String, GuiMenu*> it = m_menus.find(tag);
		if (it == m_menus.end()) {
			m_menus[tag] = new GuiMenu(tag, label);
			AddChildLayout(m_menus[tag]);
		}
		return m_menus[tag];
	}

	void GuiMenuBar::RemoveMenu(const String& tag) {
		MapIterator<String, GuiMenu*> it = m_menus.find(tag);
		if (it != m_menus.end()) {
			RemoveChildLayout(tag);
			DELETE_OBJECT(m_menus[tag]);
			m_menus.erase(tag);
		}
	}

	void GuiMenuBar::Render(void* layoutData) {
		if (ImGui::BeginMenuBar()) {
			RenderChildLayouts(layoutData);
			ImGui::EndMenuBar();
		}
	}
}