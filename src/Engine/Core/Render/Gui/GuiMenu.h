#ifndef GUIMENU
#define GUIMENU

#include "Engine/Core/Render/Gui/GuiLayout.h"
#include "Engine/Core/Render/Gui/GuiMenuItem.h"

namespace Engine {
	class GuiMenu : public GuiLayout {
	private:
		String m_label;

		Map<String, GuiMenuItem*> m_menuItems;

	public:
		GuiMenu(const String& tag, const String& label);
		virtual ~GuiMenu();

		String GetLabel() const;

		GuiMenuItem* AddMenuItem(const String& tag, const String& label);
		void RemoveMenuItem(const String& tag);

	private:
		void Render(void* layoutData) override;
	};
}

#endif // !GUIMENU
