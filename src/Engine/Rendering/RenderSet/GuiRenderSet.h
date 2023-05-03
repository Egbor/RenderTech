#ifndef GUIRENDERSET
#define GUIRENDERSET

#include "Engine/Rendering/RenderSet/RenderSet.h"
#include "Engine/Rendering/Gui/MainWindow.h"

namespace Engine {
	class GuiRenderSet : public RenderSet {
	private:
		MainWindow* m_rootLayout;

	public:
		GuiRenderSet(GuiContext* context, Texture2D* target);
		virtual ~GuiRenderSet();

		Texture2D* GetViewportTarget() const;
	};
}

#endif // !GUIRENDERSET