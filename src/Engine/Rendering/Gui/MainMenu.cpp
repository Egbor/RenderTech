#include "Engine/Rendering/Gui/MainMenu.h"

namespace Engine {
	MainMenu::MainMenu(const String& tag) 
		: GuiMenuBar(tag) {		
		GuiMenu* menuFile = AddMenu("File", "File");
		{
			menuFile->AddMenuItem("Exit", "Exit")->AddOnClickEvent(Delegate<MainMenu, GuiLayout*>::Allocate(this, &MainMenu::OnClickCloseMenuItem));
		}

		GuiMenu* menuScene = AddMenu("Scene", "Scene");
		{
			menuScene->AddMenuItem("AddObject", "Add object");
			menuScene->AddMenuItem("AddPointLight", "Add point light");
		}
	}

	void MainMenu::OnClickCloseMenuItem(GuiLayout* owner) {
		exit(0);
	}
}