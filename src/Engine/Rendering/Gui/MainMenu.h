#ifndef MAINMENU
#define MAINMENU

#include "Engine/Core/Render/Gui/GuiMenuBar.h"

namespace Engine {
	class MainMenu : public GuiMenuBar {
	public:
		MainMenu(const String& tag = "MainMenu");
		virtual ~MainMenu() = default;

		void OnClickCloseMenuItem(GuiLayout* owner);
	};
}

#endif // !MAINMENU
