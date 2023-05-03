#include "Engine/Rendering/Gui/Viewport.h"

#include <sstream>
#include <Windows.h>

namespace Engine {
	Viewport::Viewport(const String& name, const String& tag)
		: GuiWidget(tag, name), m_image(new GuiImage(tag + "1")) {
		AddChildLayout(m_image);
		AddOnResizeEvent(Delegate<Viewport, GuiLayout*, Int32, Int32>::Create(this, &Viewport::OnResizeEvent));
	}

	Viewport::~Viewport() {
		DELETE_LAYOUT(m_image);
	}

	void Viewport::SetTexture(GuiContext* context, Texture2D* texture) {
		m_image->SetTexture(context, texture);
	}

	void Viewport::OnResizeEvent(GuiLayout* owner, Int32 width, Int32 height) {
		m_image->Resize(width, height);
	}
}