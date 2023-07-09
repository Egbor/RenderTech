#ifndef GUIRENDERSET
#define GUIRENDERSET

#include "Engine/Rendering/RenderSet/RenderSet.h"
#include "Engine/Rendering/Gui/MainWindow.h"

namespace Engine {
	class GuiRenderSet : public RenderSet {
	private:
		MainWindow* m_rootLayout;
		Texture2D* m_viewportTarget;

	public:
		GuiRenderSet(GuiContext* context, Texture2D* target);
		virtual ~GuiRenderSet();

		Texture2D* GetViewportTarget() const;

		void AddViewportResizeEvent(EventBase<Int32, Int32>& callback);
		void RemoveViewportResizeEvent(EventBase<Int32, Int32>& callback);

		void AddOnAddObjectEvent(EventBase<const String&>& callback);
		void RemoveOnAddObjectEvent(EventBase<const String&>& callback);

		void AddEntityToExplorer(Entity* entity);
		void RemoveEntityFromExplorer(Entity* entity);
	};
}

#endif // !GUIRENDERSET
