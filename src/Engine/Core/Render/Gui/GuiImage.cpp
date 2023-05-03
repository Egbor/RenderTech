#include "Engine/Core/Render/Gui/GuiImage.h"

#include "imgui/imgui.h"

namespace Engine {
	GuiImage::GuiImage(const String& tag)
		: GuiLayout(tag), m_id(nullptr), 
		m_width(800), m_height(600), m_maxWidth(m_width), m_maxHeight(m_height) {
	}

	Int32 GuiImage::GetWidth() const {
		return m_width;
	}

	Int32 GuiImage::GetHeight() const {
		return m_height;
	}

	void GuiImage::SetTexture(GuiContext* context, Texture2D* texture) {
		context->AllocateGuiImageResource(texture, &m_id);
		if (texture != nullptr) {
			m_maxWidth = texture->GetWidth();
			m_maxHeight = texture->GetHeight();
		}
	}

	void GuiImage::Resize(Int32 width, Int32 height) {
		m_width = width;
		m_height = height;
	}

	void GuiImage::Render() {
		if (m_id != nullptr) {
			Float x = m_width / (Float)m_maxWidth;
			Float y = m_height / (Float)m_maxHeight;
			ImGui::Image(m_id, ImVec2(m_width, m_height), ImVec2(0.0f, 0.0f), ImVec2(x, y));
		}
	}
}