#include "Engine/Rendering/Gui/MainMenu.h"

#include <windows.h>

namespace Engine {
	MainMenu::MainMenu(const String& tag) 
		: GuiMenuBar(tag) {		
		GuiMenu* menuFile = AddMenu("File", "File");
		{
			menuFile->AddMenuItem("Exit", "Exit")->AddOnClickEvent(Delegate<MainMenu, GuiLayout*>::Allocate(this, &MainMenu::OnClickCloseMenuItem));
		}

		GuiMenu* menuScene = AddMenu("Scene", "Scene");
		{
			menuScene->AddMenuItem("AddObject", "Add object")->AddOnClickEvent(Delegate<MainMenu, GuiLayout*>::Allocate(this, &MainMenu::OnClickAddObject));
			menuScene->AddMenuItem("AddPointLight", "Add point light")->AddOnClickEvent(Delegate<MainMenu, GuiLayout*>::Allocate(this, &MainMenu::OnClickAddObject));
		}
	}

	void MainMenu::AddOnAddObjectEvent(EventBase<const String&>& callback) {
		m_eventOnAddObjectEvent += callback;
	}

	void MainMenu::RemoveOnAddObjectEvent(EventBase<const String&>& callback) {
		m_eventOnAddObjectEvent -= callback;
	}

	void MainMenu::OnClickCloseMenuItem(GuiLayout* owner) {
		exit(0);
	}

	void MainMenu::OnClickAddObject(GuiLayout* owner) {
		TCHAR filename[256] = { 0 };
		TCHAR modelname[256] = { 0 };

		OPENFILENAME openFileName;
		ZeroMemory(&openFileName, sizeof(OPENFILENAME));
		openFileName.lStructSize = sizeof(OPENFILENAME);
		openFileName.hwndOwner = NULL;//FindWindowA("RenderTechClass", "RenderTech");
		openFileName.lpstrFile = filename;
		openFileName.nMaxFile = 256;
		openFileName.lpstrFilter = "RenderTech Object(.rto)\0*.rto\0";
		openFileName.nFilterIndex = 1;
		openFileName.lpstrFileTitle = modelname;
		openFileName.nMaxFileTitle = 256;
		openFileName.lpstrInitialDir = NULL;
		openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (GetOpenFileName(&openFileName)) {
			m_eventOnAddObjectEvent(openFileName.lpstrFile);
		}
	}
}