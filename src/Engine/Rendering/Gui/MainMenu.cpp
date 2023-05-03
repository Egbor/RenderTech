#include "Engine/Rendering/Gui/MainMenu.h"

namespace Engine {
	MainMenu::MainMenu(const String& tag) 
		: GuiMenuBar(tag) {
		GuiMenu* menuFile = AddMenu("File", "File");
		{
			menuFile->AddMenuItem("Exit", "Exit")->AddOnClickEvent(Delegate<MainMenu, GuiLayout*>::Create(this, &MainMenu::OnClickCloseMenuItem));
		}
	}

	void MainMenu::OnClickCloseMenuItem(GuiLayout* owner) {

	}
}