#ifndef GUIMENUBAR
#define GUIMENUBAR

#include "Engine/Core/Render/Gui/GuiLayout.h"
#include "Engine/Core/Render/Gui/GuiMenu.h"

namespace Engine {
	class GuiMenuBar : public GuiLayout {
	private:
		Map<String, GuiMenu*> m_menus;

	public:
		GuiMenuBar(const String& tag);
		virtual ~GuiMenuBar();

		 GuiMenu* AddMenu(const String& tag, const String& label);
		 void RemoveMenu(const String& tag);

	private:
		void Render() override;
	};
}

#endif // !GUIMENUBAR