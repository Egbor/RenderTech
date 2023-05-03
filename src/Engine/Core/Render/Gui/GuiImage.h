#ifndef GUIIMAGE
#define GUIIMAGE

#include "Engine/Core/Render/Gui/GuiLayout.h"
#include "Engine/Core/Render/Gui/GuiContext.h"
#include "Engine/Object/Class/Target.h"

namespace Engine {
	class GuiImage : public GuiLayout {
	private:
		GuiResourceId m_id;

		Int32 m_width;
		Int32 m_height;
		Float m_maxWidth;
		Float m_maxHeight;

	public:
		GuiImage(const String& tag);
		virtual ~GuiImage() = default;

		Int32 GetWidth() const;
		Int32 GetHeight() const;

		void SetTexture(GuiContext* context, Texture2D* texture);
		void Resize(Int32 width, Int32 height);

	private:
		void Render() override;
	};
}

#endif // !GUIIMAGE
