#ifndef MAINWINDOW
#define MAINWINDOW

#include "Engine/Core/Render/Gui/GuiWindow.h"

#include "Engine/Rendering/Gui/MainMenu.h"
#include "Engine/Rendering/Gui/Viewport.h"
#include "Engine/Rendering/Gui/WorldExplorer.h"

namespace Engine {
	class MainWindow : public GuiWindow {
	private:
		MainMenu* m_mainMenu;
		Viewport* m_viewport;
		WorldExplorer* m_worldExplorer;

	public:
		MainWindow(GuiContext* context, const String& tag = "MainWindow");
		virtual ~MainWindow();

		Texture2D* GetViewportTarget() const;
	};
}

#endif // !MAINWINDOW
