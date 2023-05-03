#include "Engine/Core/Render/Gui/GuiImage.h"

#include "imgui/imgui.h"

namespace Engine {
	GuiImage::GuiImage(const String& tag, GuiContext* context, Int32 width, Int32 height, TextureFormat format)
		: GuiLayout(tag), m_context(context), m_texture(nullptr) {
		m_texture = m_context->GetContext()->CreateTexture2D(tagTargetTexture, { (UInt32)width, (UInt32)height, format });
	}

	Int32 GuiImage::GetWidth() const {
		return m_texture->GetWidth();
	}

	Int32 GuiImage::GetHeight() const {
		return m_texture->GetHeight();
	}

	Texture2D* GuiImage::GetTarget() const {
		return m_texture;
	}

	void GuiImage::Resize(Int32 width, Int32 height, TextureFormat format) {
		m_texture->Resize(width, height, format);
	}

	void GuiImage::Render() {
		static GuiResourceId imageId = nullptr;

		m_context->AllocateGuiImageResource(m_texture, &imageId);
		ImGui::Image(imageId, ImVec2(m_texture->GetWidth(), m_texture->GetHeight()));
	}
}