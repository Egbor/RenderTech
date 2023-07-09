#ifndef MAINWINDOW
#define MAINWINDOW

#include "Engine/Core/Render/Gui/GuiWindow.h"

#include "Engine/Rendering/Gui/MainMenu.h"
#include "Engine/Rendering/Gui/Viewport.h"
#include "Engine/Rendering/Gui/WorldExplorer.h"
#include "Engine/Rendering/Gui/ComponentExplorer.h"
#include "Engine/Rendering/Gui/Properties.h"

namespace Engine {
	class MainWindow : public GuiWindow {
	private:
		MainMenu* m_mainMenu;
		Viewport* m_viewport;
		WorldExplorer* m_worldExplorer;
		ComponentExplorer* m_componentExplorer;
		Properties* m_properties;

		Event<Int32, Int32> m_eventOnViewportResize;

	public:
		MainWindow(const String& tag = "MainWindow");
		virtual ~MainWindow();

		void SetViewportTexture(GuiContext* context, Texture2D* texture);

		void AddEntityToExplorer(Entity* entity);
		void RemoveEntityFromExplorer(Entity* entity);

		void AddViewportResizeEvent(EventBase<Int32, Int32>& callback);
		void RemoveViewportResizeEvent(EventBase<Int32, Int32>& callback);

		void AddOnAddObjectEvent(EventBase<const String&>& callback);
		void RemoveOnAddObjectEvent(EventBase<const String&>& callback);

	private:
		void OnEntitySelected(GuiLayout* owner, GuiTree* node);
		void OnComponentSelected(GuiLayout* owner, GuiTree* node);
		void OnViewportResize(GuiLayout* owner, Int32 width, Int32 height);
	};
}

#endif // !MAINWINDOW
