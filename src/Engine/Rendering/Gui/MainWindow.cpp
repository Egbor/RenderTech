#include "Engine/Rendering/Gui/MainWindow.h"

#include "Engine/Object/Global/EngineConfig.h"

namespace Engine {
	MainWindow::MainWindow(GuiContext* context, const String& tag) 
		: GuiWindow(tag), m_mainMenu(new MainMenu()),
		m_viewport(new Viewport(context, "Viewport")), m_worldExplorer(new WorldExplorer("World")) {
		AddChildLayout(m_mainMenu);
		AddWidget(m_viewport, GuiSplitDirection::GSD_LEFT, 0.8f);
		AddWidget(m_worldExplorer, GuiSplitDirection::GSD_NONE, 0.0f);
	}

	MainWindow::~MainWindow() {
		DELETE_LAYOUT(m_mainMenu);
		DELETE_LAYOUT(m_viewport);
		DELETE_LAYOUT(m_worldExplorer);
	}

	Texture2D* MainWindow::GetViewportTarget() const {
		return m_viewport->GetTarget();
	}
}