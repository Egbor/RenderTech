#include "Engine/Rendering/Gui/Viewport.h"

#include <sstream>
#include <Windows.h>

namespace Engine {
	Viewport::Viewport(GuiContext* context, const String& name, const String& tag)
		: GuiWidget(tag, name), m_image(new GuiImage(tag + "1", context, GetWidth(), GetHeight(), TextureFormat::TF_R8G8B8A8_BMP)) {
		AddOnResizeEvent(Delegate<Viewport, GuiLayout*, Int32, Int32>::Create(this, &Viewport::OnViewportResize));
		AddChildLayout(m_image);
	}

	Viewport::~Viewport() {
		DELETE_LAYOUT(m_image);
	}

	Texture2D* Viewport::GetTarget() const {
		return m_image->GetTarget();
	}

	void Viewport::OnViewportResize(GuiLayout* owner, Int32 width, Int32 height) {
		m_image->Resize(width, height, TextureFormat::TF_R8G8B8A8_BMP);
	}
}