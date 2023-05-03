#include "Engine/Rendering/Gui/MainWindow.h"

#include "Engine/Object/Global/EngineConfig.h"

namespace Engine {
	MainWindow::MainWindow(const String& tag) 
		: GuiWindow(tag), m_mainMenu(new MainMenu()),
		m_viewport(new Viewport("Viewport")), m_worldExplorer(new WorldExplorer("World")) {
		AddChildLayout(m_mainMenu);
		AddWidget(m_viewport, GuiSplitDirection::GSD_LEFT, 0.8f);
		AddWidget(m_worldExplorer, GuiSplitDirection::GSD_NONE, 0.0f);

		m_viewport->AddOnResizeEvent(Delegate<MainWindow, GuiLayout*, Int32, Int32>::Create(this, &MainWindow::OnViewportResize));
	}

	MainWindow::~MainWindow() {
		DELETE_LAYOUT(m_mainMenu);
		DELETE_LAYOUT(m_viewport);
		DELETE_LAYOUT(m_worldExplorer);
	}

	void MainWindow::SetViewportTexture(GuiContext* context, Texture2D* texture) {
		m_viewport->SetTexture(context, texture);
	}

	void MainWindow::AddViewportResizeEvent(EventBase<Int32, Int32>& callback) {
		m_eventOnViewportResize += callback;
	}

	void MainWindow::RemoveViewportResizeEvent(EventBase<Int32, Int32>& callback) {
		m_eventOnViewportResize -= callback;
	}

	void MainWindow::OnViewportResize(GuiLayout* owner, Int32 width, Int32 height) {
		m_eventOnViewportResize(width, height);
	}
}