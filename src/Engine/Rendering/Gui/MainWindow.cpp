#include "Engine/Rendering/Gui/MainWindow.h"

#include "Engine/Object/Global/EngineConfig.h"

namespace Engine {
	MainWindow::MainWindow(const String& tag) 
		: GuiWindow(tag), m_mainMenu(new MainMenu())
		, m_viewport(new Viewport("Viewport")), m_worldExplorer(new WorldExplorer("World"))
		, m_properties(new Properties("Properties")) {
		AddChildLayout(m_mainMenu);
		AddWidget(m_properties, GuiSplitDirection::GSD_NONE, 0.0f);
		AddWidget(m_worldExplorer, GuiSplitDirection::GSD_UP, 0.5f);
		AddWidget(m_viewport, GuiSplitDirection::GSD_LEFT, 0.8f);

		m_viewport->AddOnResizeEvent(Delegate<MainWindow, GuiLayout*, Int32, Int32>::Allocate(this, &MainWindow::OnViewportResize));
		m_worldExplorer->AddOnEntitySelectedEvent(Delegate<MainWindow, GuiLayout*, GuiTreeNode*>::Allocate(this, &MainWindow::OnEntitySelected));
	}

	MainWindow::~MainWindow() {
		DELETE_LAYOUT(m_mainMenu);
		DELETE_LAYOUT(m_viewport);
		DELETE_LAYOUT(m_worldExplorer);
		DELETE_LAYOUT(m_properties);
	}

	void MainWindow::AddEntityToExplorer(Entity* entity) {
		m_worldExplorer->AddEntityToExplorer(entity);
	}

	void MainWindow::RemoveEntityFromExplorer(Entity* entity) {
		m_worldExplorer->RemoveEntityFromExplorer(entity);
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

	void MainWindow::OnEntitySelected(GuiLayout* owner, GuiTreeNode* node) {
		m_properties->SetEntity(reinterpret_cast<Entity*>(node->GetId()));
	}

	void MainWindow::OnViewportResize(GuiLayout* owner, Int32 width, Int32 height) {
		m_eventOnViewportResize(width, height);
	}
}