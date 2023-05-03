#ifndef VIEWPORT
#define VIEWPORT

#include "Engine/Core/Render/Gui/GuiWidget.h"
#include "Engine/Core/Render/Gui/GuiImage.h"

namespace Engine {
	class Viewport : public GuiWidget {
	private:
		GuiImage* m_image;

	public:
		Viewport(const String& name, const String& tag = "Viewport");
		virtual ~Viewport();

		void SetTexture(GuiContext* context, Texture2D* texture);

	private:
		void OnResizeEvent(GuiLayout* owner, Int32 width, Int32 height);
	};
}

#endif // !VIEWPORT
